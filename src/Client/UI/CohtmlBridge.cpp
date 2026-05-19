// CohtmlBridge.cpp
// Hooks cohtml::View::Load (or ::LoadURL) to capture the live View* and then
// calls cohtml::View::ExecuteScript for JS injection into MC menu screens.
//
// Hook strategy (in order of preference, falling back as needed):
//  1. GetProcAddress the mangled Load export from cohtml.WindowsDesktop.dll.
//  2. If that fails, walk the DLL's export table looking for any "Load" symbol
//     whose mangled name contains "@View@cohtml@@".
//  3. Last resort: return nullptr for the hook target and set g_hookFailed so
//     the MCP tools report a useful "not available" error.
//
// cohtml::View::ExecuteScript(const char* script, bool delayUntilLoad = false)
// is similarly resolved via export name or a table scan.
//
// All calls into cohtml go through __try/__except (POD-inner pattern per project
// conventions — see MCPServer.cpp / Waila.cpp for precedent).

#include "CohtmlBridge.hpp"

#ifndef __RELEASE__

#include <atomic>
#include <cstdint>
#include <Windows.h>
#include <minhook/MinHook.h>

#include "../../Utils/Logger/Logger.hpp"

// ---------------------------------------------------------------------------
// Helpers — must be forward-declared before the __try bodies so that
// destructible types never live in the same frame as __try (__C2712 guard).
// ---------------------------------------------------------------------------

namespace {

// ---- forward declarations -------------------------------------------------
static bool tryInstallHooks();

// ---- global state ---------------------------------------------------------
static std::atomic<void*>   g_view{nullptr};       // captured cohtml::View*
static std::atomic<bool>    g_hookInstalled{false};
static std::atomic<bool>    g_hookFailed{false};
static std::atomic<bool>    g_ready{false};

// cohtml::View::ExecuteScript — resolved from the DLL export table.
// Signature: void __thiscall ExecuteScript(cohtml::View*, const char* script, bool delayUntilLoad)
// ABI note: member-function => x64 __fastcall with implicit first arg = this.
using ExecuteScript_t = void(__fastcall*)(void* view, const char* script, bool delay);
static ExecuteScript_t g_executeScript = nullptr;        // direct call entry (post-hook = trampoline)
static ExecuteScript_t g_executeScriptOrig = nullptr;    // MinHook-provided trampoline

// Original pointer for the hooked Load function.
using ViewLoad_t = void(__fastcall*)(void* view, const char* url, int flags);
static ViewLoad_t g_viewLoadOrig = nullptr;

// cohtml::View::Advance — called every frame by MC regardless of what's on screen.
// This makes it the ideal hook for capturing g_view on frame 0, before any ExecuteScript
// fires (which never happens on the title screen whose cohtml is static/no-JS).
//
// The implementation dispatched to from the View vtable is at RVA 0x467160.
// MC calls through the vtable slot 0 thunk (RVA 0x47DE60, adjustor: ADD RCX,0x48) which
// forwards to this function. The detour receives "inner_this" in RCX — the same pointer
// that ExecuteScript expects (confirmed by matching field guards: ExecScript checks
// [RCX+0x220] and Advance's recursion guard checks [RCX+0x4E0], both on the same object).
//
// Function signature (x64 fastcall, thiscall convention):
//   void View::Advance(uint64_t timeMs)   — RCX=inner_this, RDX=timeMs
// (The function saves RDX at [RSP+10] on entry, confirming the uint64 arg.)
//
// Byte signature (32 bytes, unique in .text of cohtml.WindowsDesktop.dll 1.26.2101.0):
//   48 89 5C 24 08 4C 89 44 24 18 48 89 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D9 48
// Disassembly (first 32 bytes):
//   MOV [RSP+8],RBX          ; 48 89 5C 24 08
//   MOV [RSP+18],R8          ; 4C 89 44 24 18
//   MOV [RSP+10],RDX         ; 48 89 54 24 10   <- saves timeMs
//   PUSH RBP,RSI,RDI,R12-R15 ; 55 56 57 41 54 41 55 41 56 41 57
//   LEA RBP,[RSP-0x27]       ; 48 8D 6C 24 D9
//   ...
// Confirmed: single match in DLL; RVA 0x467160.
using ViewAdvance_t = void(__fastcall*)(void* view, uint64_t timeMs);
static ViewAdvance_t g_viewAdvanceOrig = nullptr;

// Detour on cohtml::View::ExecuteScript. MC calls this whenever it pushes JS into
// its menu UI (and similar paths). Capturing the first non-null `view` arg gives
// us a usable View* without needing View::Load to be exported. Bridge-initiated
// calls go through g_executeScriptOrig directly so they don't re-enter capture.
static void __fastcall executeScriptDetour(void* view, const char* script, bool delay) {
    // Always update — cohtml destroys/recreates views on screen transitions, so a
    // stale first-seen pointer faults when we later call ExecuteScript. Tracking
    // the most-recently-seen view keeps us pinned to the live one for each frame.
    if (view) {
        void* prev = g_view.exchange(view, std::memory_order_acq_rel);
        if (prev != view) {
            Logger::custom(fg(fmt::color::lime_green), "CohtmlBridge",
                           "Captured cohtml::View* 0x{:X} via ExecuteScript detour (prev=0x{:X})",
                           reinterpret_cast<uintptr_t>(view),
                           reinterpret_cast<uintptr_t>(prev));
        }
    }
    if (g_executeScriptOrig) g_executeScriptOrig(view, script, delay);
}

// ---- utility: scan .text section for a byte signature -------------------

// Byte-pattern scanner for cohtml.WindowsDesktop.dll's .text section.
// `sig` is a space-separated hex string where "??" is a wildcard byte.
// Returns the VA of the first match, nullptr if not found or if the DLL's
// .text section cannot be located.
//
// Rationale: cohtml.WindowsDesktop.dll does NOT export View::ExecuteScript or
// View::Load (only Library factory entrypoints and IViewListener virtuals are
// exported). This scanner is the last-resort fallback when the export table
// and export-name scan both miss.
//
// Known signature for cohtml::View::ExecuteScript
// (verified against cohtml.WindowsDesktop.dll shipped with MC 1.26.2101.0):
//   "40 57 48 81 EC 80 00 00 00 83 B9 20 02 00 00 04"
//   Disassembly:
//     PUSH RDI              ; 40 57
//     SUB  RSP, 0x80        ; 48 81 EC 80 00 00 00
//     CMP  DWORD [RCX+0x220], 4  ; 83 B9 20 02 00 00 04
//   The CMP checks `this->documentStatus == kBeingDestroyed` — a guard unique
//   to ExecuteScript.  Confirmed single match across the entire DLL.
static void* scanTextSection(HMODULE hmod, const char* sig) {
    if (!hmod || !sig) return nullptr;

    auto base = reinterpret_cast<uintptr_t>(hmod);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);

    // Parse the sig string into (byte, is_wildcard) pairs.
    // We use a fixed-size stack buffer to avoid heap allocation inside __try.
    struct BytePat { uint8_t b; bool wild; };
    static constexpr size_t kMaxPat = 64;
    BytePat pat[kMaxPat];
    size_t patLen = 0;

    const char* p = sig;
    while (*p && patLen < kMaxPat) {
        while (*p == ' ') ++p;
        if (!*p) break;
        if (p[0] == '?' && p[1] == '?') {
            pat[patLen++] = { 0, true };
            p += 2;
        } else {
            auto hexdig = [](char c) -> int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                return -1;
            };
            int hi = hexdig(p[0]), lo = hexdig(p[1]);
            if (hi < 0 || lo < 0) break;
            pat[patLen++] = { static_cast<uint8_t>(hi << 4 | lo), false };
            p += 2;
        }
    }
    if (patLen == 0) return nullptr;

    // Locate the .text section.
    auto* sectionHdr = IMAGE_FIRST_SECTION(nt);
    const uint8_t* textBase = nullptr;
    size_t textSize = 0;
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++sectionHdr) {
        if (memcmp(sectionHdr->Name, ".text", 5) == 0) {
            textBase = reinterpret_cast<const uint8_t*>(base + sectionHdr->VirtualAddress);
            textSize = sectionHdr->Misc.VirtualSize;
            break;
        }
    }
    if (!textBase || textSize < patLen) return nullptr;

    const uint8_t* end = textBase + textSize - patLen;
    for (const uint8_t* cur = textBase; cur <= end; ++cur) {
        bool match = true;
        for (size_t j = 0; j < patLen; ++j) {
            if (!pat[j].wild && cur[j] != pat[j].b) { match = false; break; }
        }
        if (match) return const_cast<void*>(reinterpret_cast<const void*>(cur));
    }
    return nullptr;
}

// ---- utility: scan export table ------------------------------------------

// Walk the export directory of a loaded module looking for a symbol whose name
// contains `needle`. Returns the function VA on match, nullptr if not found.
static void* scanExportTable(HMODULE hmod, const char* needle) {
    if (!hmod) return nullptr;
    auto base = reinterpret_cast<uintptr_t>(hmod);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;
    auto* nt  = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    auto& expDir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (expDir.VirtualAddress == 0) return nullptr;
    auto* exp = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base + expDir.VirtualAddress);
    auto* names    = reinterpret_cast<DWORD*>(base + exp->AddressOfNames);
    auto* ordinals = reinterpret_cast<WORD*> (base + exp->AddressOfNameOrdinals);
    auto* funcs    = reinterpret_cast<DWORD*>(base + exp->AddressOfFunctions);
    for (DWORD i = 0; i < exp->NumberOfNames; ++i) {
        const char* name = reinterpret_cast<const char*>(base + names[i]);
        if (strstr(name, needle)) {
            DWORD rva = funcs[ordinals[i]];
            return reinterpret_cast<void*>(base + rva);
        }
    }
    return nullptr;
}

// ---- inner ExecuteScript wrapper (POD-only, safe with __try) -------------

struct ExecArgs {
    void*       view;
    const char* script;
    bool        delay;
};

static void executeScriptInner(const ExecArgs* a) {
    // Prefer the MinHook trampoline if our detour is installed — calling the
    // detour'd entry would re-enter our capture logic and risk pollution.
    if (g_executeScriptOrig) g_executeScriptOrig(a->view, a->script, a->delay);
    else                     g_executeScript(a->view, a->script, a->delay);
}

static bool executeScriptSafe(void* view, const char* script, bool delay) {
    ExecArgs a{ view, script, delay };
    __try {
        executeScriptInner(&a);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

// ---- View::Advance detour ------------------------------------------------

// Called every frame by MC (via vtable slot 0 thunk). Captures the live View*
// into g_view on first call so that executeScript() works immediately — even on
// the title screen whose static cohtml never fires an ExecuteScript of its own.
//
// RCX here = "inner_this" (outer View object + 0x48 offset, per the adjustor thunk).
// This is the same pointer that ExecuteScript checks at [RCX+0x220], so g_view
// populated here is directly usable for later ExecuteScript calls.
static void __fastcall viewAdvanceDetour(void* view, uint64_t timeMs) {
    if (view) {
        void* prev = g_view.exchange(view, std::memory_order_acq_rel);
        if (prev != view) {
            Logger::custom(fg(fmt::color::lime_green), "CohtmlBridge",
                           "Captured cohtml::View* 0x{:X} via Advance detour "
                           "(prev=0x{:X})",
                           reinterpret_cast<uintptr_t>(view),
                           reinterpret_cast<uintptr_t>(prev));
        }
    }
    if (g_viewAdvanceOrig) g_viewAdvanceOrig(view, timeMs);
}

// ---- View::Load detour ---------------------------------------------------

static void __fastcall viewLoadDetour(void* view, const char* url, int flags) {
    // Capture the first non-null view we see.
    if (view && !g_view.load(std::memory_order_relaxed)) {
        g_view.store(view, std::memory_order_release);
        Logger::custom(fg(fmt::color::lime_green), "CohtmlBridge",
                       "Captured cohtml::View* 0x{:X} (url={})",
                       reinterpret_cast<uintptr_t>(view),
                       url ? url : "<null>");
    }
    // Forward — never eat the call.
    if (g_viewLoadOrig) {
        g_viewLoadOrig(view, url, flags);
    }
}

// ---- inner hook-install (no C++ destructible locals, __try-safe) ---------

struct ResolvedSyms {
    void* loadAddr;
    void* execAddr;
};

static bool resolveSymbols(HMODULE hmod, ResolvedSyms* out) {
    out->loadAddr = nullptr;
    out->execAddr = nullptr;

    // --- ExecuteScript ---
    // Try the most common x64 mangled names first.
    static const char* execCandidates[] = {
        "?ExecuteScript@View@cohtml@@QEAAXPEBD_N@Z",
        "?ExecuteScript@View@cohtml@@QEAAXPEBDH@Z",
        nullptr
    };
    for (int i = 0; execCandidates[i]; ++i) {
        out->execAddr = GetProcAddress(hmod, execCandidates[i]);
        if (out->execAddr) break;
    }
    if (!out->execAddr) {
        // Fall back to export-table scan.
        out->execAddr = scanExportTable(hmod, "ExecuteScript@View@cohtml@@");
    }

    // --- View::Load ---
    static const char* loadCandidates[] = {
        "?Load@View@cohtml@@QEAAXPEBDH@Z",
        "?Load@View@cohtml@@QEAAXPEBD@Z",
        "?LoadURL@View@cohtml@@QEAAXPEBDH@Z",
        nullptr
    };
    for (int i = 0; loadCandidates[i]; ++i) {
        out->loadAddr = GetProcAddress(hmod, loadCandidates[i]);
        if (out->loadAddr) break;
    }
    if (!out->loadAddr) {
        out->loadAddr = scanExportTable(hmod, "Load@View@cohtml@@");
    }

    return (out->execAddr != nullptr); // Load is optional; Exec is required for usefulness
}

static bool tryInstallHooks() {
    if (g_hookInstalled.load(std::memory_order_relaxed)) return true;

    HMODULE hmod = GetModuleHandleA("cohtml.WindowsDesktop.dll");
    if (!hmod) {
        // DLL not yet loaded — will retry on next initialize() call.
        return false;
    }

    ResolvedSyms syms{};
    if (!resolveSymbols(hmod, &syms)) {
        // Export table + export-name scan both missed (expected: cohtml ships no
        // View member exports). Try the .text-section byte-pattern fallback.
        //
        // Signature "40 57 48 81 EC 80 00 00 00 83 B9 20 02 00 00 04" was derived
        // by static analysis of cohtml.WindowsDesktop.dll (MC 1.26.2101.0 x64):
        //   PUSH RDI ; SUB RSP,0x80 ; CMP [RCX+0x220],4
        // The CMP is the document-being-destroyed guard unique to ExecuteScript.
        // Confirmed single match across the entire DLL (RVA 0x34AA20).
        static constexpr const char* k_execScriptSig =
            "40 57 48 81 EC 80 00 00 00 83 B9 20 02 00 00 04";

        void* execViaText = scanTextSection(hmod, k_execScriptSig);
        if (execViaText) {
            auto execRva = reinterpret_cast<uintptr_t>(execViaText)
                         - reinterpret_cast<uintptr_t>(hmod);
            Logger::custom(fg(fmt::color::orange), "CohtmlBridge",
                           "ExecuteScript resolved via .text scan at 0x{:X} "
                           "(sig: '{}')",
                           reinterpret_cast<uintptr_t>(execViaText),
                           k_execScriptSig);
            (void)execRva; // logged above; kept for potential future use
            syms.execAddr = execViaText;
        } else {
            Logger::error("[CohtmlBridge] cohtml.WindowsDesktop.dll loaded but "
                          "ExecuteScript not found in exports or .text sig scan; "
                          "sig tried: '{}'", k_execScriptSig);
            g_hookFailed.store(true, std::memory_order_relaxed);
            return false;
        }
    }
    g_executeScript = reinterpret_cast<ExecuteScript_t>(syms.execAddr);
    Logger::custom(fg(fmt::color::deep_sky_blue), "CohtmlBridge",
                   "ExecuteScript resolved at 0x{:X}",
                   reinterpret_cast<uintptr_t>(syms.execAddr));

    // Hook ExecuteScript itself — when MC calls into it for its own UI scripting,
    // the detour grabs the View* arg. Primary view-capture path on builds where
    // View::Load isn't exported.
    {
        MH_STATUS es = MH_CreateHook(syms.execAddr,
                                     reinterpret_cast<void*>(&executeScriptDetour),
                                     reinterpret_cast<void**>(&g_executeScriptOrig));
        if (es == MH_OK || es == MH_ERROR_ALREADY_CREATED) {
            MH_QueueEnableHook(syms.execAddr);
            MH_ApplyQueued();
            Logger::custom(fg(fmt::color::deep_sky_blue), "CohtmlBridge",
                           "Hooked ExecuteScript at 0x{:X} for View* capture",
                           reinterpret_cast<uintptr_t>(syms.execAddr));
        } else {
            Logger::warn("[CohtmlBridge] MH_CreateHook(ExecuteScript) failed (MH_STATUS={})",
                         (int)es);
        }
    }

    // ---- View::Advance hook (every-frame view-capture) ----------------------
    //
    // cohtml::View::Advance is called by MC on every render frame regardless of
    // what's displayed (title screen, game, menus). Hooking it fills g_view on
    // frame 0 — before any ExecuteScript fires. This is the primary fix for the
    // "no View* on title screen" problem.
    //
    // Signature (32 bytes, unique in cohtml.WindowsDesktop.dll 1.26.2101.0):
    //   48 89 5C 24 08 4C 89 44 24 18 48 89 54 24 10 55
    //   56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D9 48
    // RVA: 0x467160. The vtable adjustor thunk at RVA 0x47DE60 (ADD RCX,0x48)
    // dispatches here. Detour receives inner_this (= outer View* + 0x48) which
    // is the same pointer that ExecuteScript uses.
    {
        static constexpr const char* k_advanceSig =
            "48 89 5C 24 08 4C 89 44 24 18 48 89 54 24 10 55 "
            "56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D9 48";

        void* advanceAddr = scanTextSection(hmod, k_advanceSig);
        if (advanceAddr) {
            auto advRva = reinterpret_cast<uintptr_t>(advanceAddr)
                        - reinterpret_cast<uintptr_t>(hmod);
            Logger::custom(fg(fmt::color::orange), "CohtmlBridge",
                           "Hooked Advance at 0x{:X} (RVA 0x{:X})",
                           reinterpret_cast<uintptr_t>(advanceAddr), advRva);

            MH_STATUS adv = MH_CreateHook(advanceAddr,
                                          reinterpret_cast<void*>(&viewAdvanceDetour),
                                          reinterpret_cast<void**>(&g_viewAdvanceOrig));
            if (adv == MH_OK || adv == MH_ERROR_ALREADY_CREATED) {
                MH_QueueEnableHook(advanceAddr);
                MH_ApplyQueued();
            } else {
                Logger::warn("[CohtmlBridge] MH_CreateHook(Advance) failed (MH_STATUS={})",
                             (int)adv);
            }
        } else {
            Logger::warn("[CohtmlBridge] View::Advance not found via sig scan "
                         "(sig: '{}'); title-screen View* capture unavailable. "
                         "ExecuteScript hook is still active for dynamic screens.",
                         k_advanceSig);
        }
    }

    if (syms.loadAddr) {
        MH_STATUS mhStatus = MH_CreateHook(syms.loadAddr,
                                           reinterpret_cast<void*>(&viewLoadDetour),
                                           reinterpret_cast<void**>(&g_viewLoadOrig));
        if (mhStatus == MH_OK || mhStatus == MH_ERROR_ALREADY_CREATED) {
            MH_QueueEnableHook(syms.loadAddr);
            MH_ApplyQueued();
            Logger::custom(fg(fmt::color::deep_sky_blue), "CohtmlBridge",
                           "Hooked View::Load at 0x{:X}",
                           reinterpret_cast<uintptr_t>(syms.loadAddr));
        } else {
            Logger::warn("[CohtmlBridge] MH_CreateHook failed for View::Load (MH_STATUS={}); "
                         "View* capture disabled — call rawViewPtr() won't work until a Load fires naturally",
                         (int)mhStatus);
        }
    } else {
        Logger::warn("[CohtmlBridge] View::Load not found in exports; "
                     "View* capture requires a Load event to fire. "
                     "ExecuteScript is still usable if a View* is set externally.");
    }

    g_hookInstalled.store(true, std::memory_order_relaxed);
    return true;
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

namespace flarial::cohtml {

void initialize() {
    static bool once = false;
    if (once) {
        // On subsequent calls (e.g. deferred retry) just attempt hook install.
        tryInstallHooks();
        return;
    }
    once = true;
    Logger::custom(fg(fmt::color::deep_sky_blue), "CohtmlBridge", "Initializing...");
    bool installed = tryInstallHooks();
    if (!installed) {
        Logger::warn("[CohtmlBridge] cohtml.WindowsDesktop.dll not yet loaded; "
                     "hook install deferred. Call initialize() again after the DLL maps, "
                     "or wait for the first executeScript() call which retries automatically.");
    }
}

bool isReady() {
    return g_ready.load(std::memory_order_relaxed);
}

std::string executeScript(const std::string& js) {
    // Lazy retry: if cohtml DLL loaded after our initialize() call.
    if (!g_hookInstalled.load(std::memory_order_relaxed) &&
        !g_hookFailed.load(std::memory_order_relaxed)) {
        tryInstallHooks();
    }
    if (g_hookFailed.load(std::memory_order_relaxed)) {
        return R"({"error":"cohtml ExecuteScript not resolved from DLL exports"})";
    }
    if (!g_executeScript) {
        return R"({"error":"ExecuteScript fn ptr null — DLL not yet loaded"})";
    }
    void* view = g_view.load(std::memory_order_acquire);
    if (!view) {
        return R"({"error":"no cohtml View captured yet — wait for a screen load"})";
    }
    bool ok = executeScriptSafe(view, js.c_str(), false);
    if (!ok) {
        // Stale view pointer — clear it so next Load event recaptures.
        g_view.store(nullptr, std::memory_order_release);
        g_ready.store(false, std::memory_order_relaxed);
        return R"({"error":"SEH fired in ExecuteScript — view pointer was stale; cleared, retry after next screen"})";
    }
    if (!g_ready.load(std::memory_order_relaxed)) {
        g_ready.store(true, std::memory_order_relaxed);
        Logger::success("[CohtmlBridge] First ExecuteScript dispatched successfully.");
    }
    return R"({"ok":true})";
}

std::string clickSelector(const std::string& sel) {
    // Escape single quotes in the selector to prevent JS injection accidents.
    std::string escaped = sel;
    size_t pos = 0;
    while ((pos = escaped.find('\'', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\'");
        pos += 2;
    }
    std::string js = "var _e=document.querySelector('" + escaped +
                     "');if(_e){_e.click();}else{console.warn('[CohtmlBridge] selector not found: " +
                     escaped + "');}";
    return executeScript(js);
}

std::string dumpDomHtml() {
    // We fire a script that stores the HTML in a global variable; then we read
    // it back. Since ExecuteScript is fire-and-forget in v1, we can't truly
    // return the DOM here — but we at least fire a console.log dump into the
    // cohtml log so the user can inspect it.
    // For v1, return a status message explaining the limitation.
    std::string js =
        "try{"
        "  var _h=document.documentElement.outerHTML;"
        "  var _t=_h.substring(0,65536);"
        "  console.log('[FlarialDom]',_t);"
        "  window.__flarialDomSnapshot=_t;"
        "}catch(e){console.error('[FlarialDom]',e.toString());}";
    std::string dispatchResult = executeScript(js);
    if (dispatchResult.find("\"ok\":true") == std::string::npos) {
        return dispatchResult;
    }
    return R"({"ok":true,"note":"DOM HTML logged via cohtml console.log (check cohtml.log). "
              "window.__flarialDomSnapshot holds the first 64KB. "
              "Real return values require v2 binding callbacks."})";
}

uintptr_t rawViewPtr() {
    return reinterpret_cast<uintptr_t>(g_view.load(std::memory_order_acquire));
}

} // namespace flarial::cohtml

#else  // __RELEASE__

// Release stubs — cohtml view-capture is part of the MCP debug surface and
// is excluded from shipping builds.

namespace flarial::cohtml {
    void initialize() {}
    bool isReady() { return false; }
    std::string executeScript(const std::string&) { return R"({"error":"cohtml bridge disabled in release"})"; }
    std::string clickSelector(const std::string&) { return R"({"error":"cohtml bridge disabled in release"})"; }
    std::string dumpDomHtml() { return R"({"error":"cohtml bridge disabled in release"})"; }
    uintptr_t rawViewPtr() { return 0; }
} // namespace flarial::cohtml

#endif // __RELEASE__
