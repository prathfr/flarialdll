#pragma once

#include <string>

// CohtmlBridge — thin wrapper around Coherent cohtml::View for MCP tooling.
//
// Call initialize() once from Client::initialize() (after HookManager::initialize()).
// The bridge defers the actual view-capture hook install until cohtml.WindowsDesktop.dll
// is loaded, so it is safe to call even if cohtml hasn't been mapped yet.
//
// Thread safety: executeScript / clickSelector / dumpDomHtml may be called from any
// thread (MCP worker). The View* is stored in a std::atomic<void*>.

namespace flarial::cohtml {

    // Install the cohtml::View::Load hook so we can capture the View*.
    // Idempotent — safe to call more than once.
    void initialize();

    // True once a non-null View* has been captured and the first ExecuteScript
    // smoke-test succeeded.
    bool isReady();

    // Run arbitrary JS in the captured cohtml view.
    // Returns {"ok":true} on success, {"error":"..."} on failure.
    // v1 is fire-and-forget; real return values require v2 binding callbacks.
    std::string executeScript(const std::string& js);

    // Convenience: document.querySelector(sel).click()
    std::string clickSelector(const std::string& sel);

    // Returns document.documentElement.outerHTML, truncated to 64 KB.
    std::string dumpDomHtml();

    // Returns the raw captured View* as uintptr_t for diagnostics.
    uintptr_t rawViewPtr();

} // namespace flarial::cohtml
