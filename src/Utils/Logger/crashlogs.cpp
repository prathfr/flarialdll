#include "crashlogs.hpp"

#include <Client/Client.hpp>
#include <Client/Module/Manager.hpp>
#include <kiero/kiero.h>
#include <Utils/CrashTelemetry.hpp>
#include <Utils/ShellMessageUtil.hpp>
#include <Utils/UserActionLogger.hpp>
#include <Utils/Utils.hpp>

//needed to get a stack trace
#include <stacktrace>

//needed for threading (threading needed to be able to output a call stack during a stack overflow)
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

//needed for being able to output a timestampped crash log
#include <chrono>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

//needed for being able to get into the crash handler on a crash
#include <csignal>
#include <exception>
#include <cstdlib>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <shellapi.h>

#pragma comment(lib, "dbghelp.lib")

#ifndef STATUS_FAIL_FAST_EXCEPTION
#define STATUS_FAIL_FAST_EXCEPTION ((DWORD)0xC0000602L)
#endif
#ifndef STATUS_HEAP_CORRUPTION
#define STATUS_HEAP_CORRUPTION ((DWORD)0xC0000374L)
#endif
#ifndef STATUS_STACK_BUFFER_OVERRUN
#define STATUS_STACK_BUFFER_OVERRUN ((DWORD)0xC0000409L)
#endif
#ifndef STATUS_FATAL_APP_EXIT
#define STATUS_FATAL_APP_EXIT ((DWORD)0x40000015L)
#endif

//a decent amount of this was copied/modified from backward.cpp (https://github.com/bombela/backward-cpp)
//mostly the stuff related to actually getting crash handlers on crashes
//and the thread which is SOLELY there to be able to write a log on a stack overflow,
//since otherwise there is not enough stack space to output the stack trace
//main difference here is utilizing C++23 <stacktrace> header for generating stack traces
//and using <atomic> and a few other more recent C++ features if we're gonna be using C++23 anyway

namespace glaiel::crashlogs {
    //information for where to save stack traces
    static std::stacktrace trace;
    static std::string header_message;
    static int crash_signal = 0; // 0 is not a valid signal id
    static std::filesystem::path output_folder;
    static std::string filename = "crash_{timestamp}.txt";
    static void (*on_output_crashlog)(std::string crashlog_filename) = NULL;

    //exception information
    static EXCEPTION_POINTERS* exception_pointers = nullptr;
    static PVOID vectored_exception_handle = nullptr;
    static DWORD crash_thread_id = 0;
    static std::string current_crash_id;
    static std::string current_report_id;
    static std::atomic_bool last_chance_crashlog_written = false;
    static std::string last_chance_crashlog_path;

    //thread stuff
    static std::mutex mut;
    static std::condition_variable cv;
    static std::thread output_thread;
    enum class program_status {
        running = 0,
        crashed = 1,
        ending = 2,
        normal_exit = 3,
        end_session = 4
    };
    static std::atomic<program_status> status = program_status::running;

    //public interface (see header for documentation)
    void set_crashlog_folder(std::string folderpath) {
        output_folder = folderpath;
    }

    // Fallback folder used when early_register() runs before set_crashlog_folder().
    // Resolves to %LOCALAPPDATA%\Flarial\Client\logs which matches Utils::getLogsPath()
    // without depending on Utils during early crash-filter registration.
    static std::filesystem::path resolve_fallback_folder() {
        wchar_t buf[MAX_PATH] = {};
        DWORD n = GetEnvironmentVariableW(L"LOCALAPPDATA", buf, MAX_PATH);
        std::filesystem::path base = (n > 0 && n < MAX_PATH)
            ? std::filesystem::path(buf)
            : std::filesystem::temp_directory_path();
        return base / L"Flarial" / L"Client" / L"logs";
    }

    static std::string current_timestamp();
    static std::filesystem::path get_log_filepath();
    static const char* try_get_signal_name(int signal);
    static std::string get_exception_name(DWORD code);
    static std::string format_hex(DWORD64 value);
    static std::string get_register_dump(CONTEXT* ctx);
    static std::string get_loaded_modules();
    static std::string get_memory_dump(void* address);
    static std::string get_exception_info();
    static std::string get_all_thread_stacks();
    static std::string format_uptime();
    static int signal_from_exception_code(DWORD exception_code);
    static bool is_vectored_fatal_exception(DWORD exception_code);
    static bool is_execute_access_violation(EXCEPTION_POINTERS* ex_ptrs);
    static bool should_capture_vectored_exception(DWORD exception_code, EXCEPTION_POINTERS* ex_ptrs);
    static void write_last_chance_crash_log(EXCEPTION_POINTERS* ex_ptrs, const char* source);
    static std::string walk_thread_stack(HANDLE hThread, CONTEXT* ctx);

    // Generate the crash dump as a string (used for both file and telemetry)
    static std::string generate_crash_dump() {
        std::stringstream log;

        // Basic information
        log << "===============================================\n";
        log << "           CRASH REPORT\n";
        log << "===============================================\n\n";
        log << "COMMIT_HASH: " << COMMIT_HASH << std::endl;
        log << "Crash ID: " << (current_crash_id.empty() ? "unavailable" : current_crash_id) << "\n";
        log << "Report ID: " << (current_report_id.empty() ? "unavailable" : current_report_id) << "\n";
        log << "Timestamp: " << current_timestamp() << "\n";
        log << "Process ID: " << GetCurrentProcessId() << "\n";
        log << "Thread ID: " << (crash_thread_id == 0 ? GetCurrentThreadId() : crash_thread_id) << "\n";
        log << "Session Uptime: " << format_uptime() << "\n";
        log << "Debugger Attached: " << (IsDebuggerPresent() ? "yes" : "no") << "\n\n";
        if (!last_chance_crashlog_path.empty()) {
            log << "Last-Chance Crashlog: " << last_chance_crashlog_path << "\n\n";
        }

        if(!header_message.empty()) {
            log << header_message << std::endl << std::endl;
        }

        log << "===============================================\n";
        log << "           USER CONTEXT\n";
        log << "===============================================\n\n";
        log << UserActionLogger::getCrashContextText() << "\n";

        // Exception/Signal information
        log << "===============================================\n";
        log << "           EXCEPTION DETAILS\n";
        log << "===============================================\n\n";

        if(crash_signal != 0) {
            log << "Signal: " << crash_signal << " (" << try_get_signal_name(crash_signal) << ")\n";
        }

        if(exception_pointers != nullptr) {
            log << get_exception_info() << "\n";
        }
        log << "\n";

        log << "===============================================\n";
        log << "           STACK TRACE (CRASHING THREAD)\n";
        log << "===============================================\n\n";

        // StackWalk64 mutates the CONTEXT, so always operate on a copy.
        if (exception_pointers && exception_pointers->ContextRecord) {
            CONTEXT ctxCopy = *exception_pointers->ContextRecord;
            log << walk_thread_stack(GetCurrentThread(), &ctxCopy);
        } else {
            CONTEXT ctx = {};
            ctx.ContextFlags = CONTEXT_FULL;
            RtlCaptureContext(&ctx);
            log << walk_thread_stack(GetCurrentThread(), &ctx);
        }
        log << "\n";

        log << "===============================================\n";
        log << "           C++ STACKTRACE (std::stacktrace)\n";
        log << "===============================================\n\n";
        {
            size_t frame_num = 0;
            for (const auto& entry : trace) {
                log << "#" << std::setw(2) << std::setfill('0') << frame_num++ << " ";
                log << entry << "\n";
            }
        }
        log << "\n";

        // Register dump
        if (exception_pointers && exception_pointers->ContextRecord) {
            log << "===============================================\n";
            log << "           REGISTER DUMP\n";
            log << "===============================================\n\n";
            log << get_register_dump(exception_pointers->ContextRecord) << "\n";
        }

        // All thread stacks
        log << "===============================================\n";
        log << "           ALL THREAD STACKS\n";
        log << "===============================================\n\n";
        log << get_all_thread_stacks() << "\n";

        // Loaded modules (image list equivalent)
        log << "===============================================\n";
        log << "           LOADED MODULES\n";
        log << "===============================================\n\n";
        log << get_loaded_modules() << "\n";

        // Memory dump around crash address
        if (exception_pointers && exception_pointers->ExceptionRecord) {
            log << "===============================================\n";
            log << "           MEMORY DUMP\n";
            log << "===============================================\n\n";
            log << get_memory_dump(exception_pointers->ExceptionRecord->ExceptionAddress) << "\n";
        }

        // Enabled Flarial modules
        log << "===============================================\n";
        log << "           ENABLED FLARIAL MODULES\n";
        log << "===============================================\n\n";

        for (const auto& pair : ModuleManager::moduleMap) {
            if(pair.second->isEnabled())
                log << "  - " << pair.second->name << "\n";
        }
        log << "\n";

        return log.str();
    }

    // Store the crash dump for telemetry access
    static std::string crash_dump_content;
    static std::string last_crashlog_path;

    static std::wstring utf8_to_wide(const std::string& input) {
        if (input.empty()) {
            return L"";
        }

        int requiredSize = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
        if (requiredSize <= 0) {
            return std::wstring(input.begin(), input.end());
        }

        std::wstring result(static_cast<size_t>(requiredSize) - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, result.data(), requiredSize);
        return result;
    }

    static const char* access_violation_operation(ULONG_PTR operation) {
        switch (operation) {
            case 0:
                return "read";
            case 1:
                return "write";
            case 8:
                return "execute";
            default:
                return "unknown";
        }
    }

    static const char* memory_state_name(DWORD state) {
        switch (state) {
            case MEM_COMMIT:
                return "MEM_COMMIT";
            case MEM_FREE:
                return "MEM_FREE";
            case MEM_RESERVE:
                return "MEM_RESERVE";
            default:
                return "UNKNOWN";
        }
    }

    static const char* memory_type_name(DWORD type) {
        switch (type) {
            case MEM_IMAGE:
                return "MEM_IMAGE";
            case MEM_MAPPED:
                return "MEM_MAPPED";
            case MEM_PRIVATE:
                return "MEM_PRIVATE";
            default:
                return "UNKNOWN";
        }
    }

    static const char* memory_protect_name(DWORD protect) {
        const DWORD baseProtect = protect & 0xff;
        switch (baseProtect) {
            case PAGE_EXECUTE:
                return "PAGE_EXECUTE";
            case PAGE_EXECUTE_READ:
                return "PAGE_EXECUTE_READ";
            case PAGE_EXECUTE_READWRITE:
                return "PAGE_EXECUTE_READWRITE";
            case PAGE_EXECUTE_WRITECOPY:
                return "PAGE_EXECUTE_WRITECOPY";
            case PAGE_NOACCESS:
                return "PAGE_NOACCESS";
            case PAGE_READONLY:
                return "PAGE_READONLY";
            case PAGE_READWRITE:
                return "PAGE_READWRITE";
            case PAGE_WRITECOPY:
                return "PAGE_WRITECOPY";
            default:
                return "UNKNOWN";
        }
    }

    static void write_last_chance_raw(HANDLE file, const char* text) {
        if (file == INVALID_HANDLE_VALUE || text == nullptr) {
            return;
        }

        DWORD written = 0;
        WriteFile(file, text, static_cast<DWORD>(std::strlen(text)), &written, nullptr);
    }

    static void write_last_chance_format(HANDLE file, const char* format, ...) {
        if (file == INVALID_HANDLE_VALUE || format == nullptr) {
            return;
        }

        char buffer[2048];
        va_list args;
        va_start(args, format);
        const int count = std::vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        if (count <= 0) {
            return;
        }

        const size_t length = static_cast<size_t>(count) >= sizeof(buffer) ? sizeof(buffer) - 1 : static_cast<size_t>(count);
        DWORD written = 0;
        WriteFile(file, buffer, static_cast<DWORD>(length), &written, nullptr);
    }

    static std::filesystem::path get_last_chance_folder() {
        try {
            if (!output_folder.empty()) {
                return output_folder;
            }

            return std::filesystem::path(Utils::getClientPath()) / "logs";
        } catch (...) {
            wchar_t tempPath[MAX_PATH] = {};
            const DWORD len = GetTempPathW(MAX_PATH, tempPath);
            if (len > 0 && len < MAX_PATH) {
                return std::filesystem::path(tempPath) / "Flarial";
            }
        }

        return std::filesystem::temp_directory_path() / "Flarial";
    }

    static std::filesystem::path make_last_chance_path() {
        SYSTEMTIME st;
        GetLocalTime(&st);

        wchar_t filenameBuffer[160];
        swprintf_s(filenameBuffer, L"crash_lastchance_%04u-%02u-%02u-%02u-%02u-%02u-p%lu-t%lu.txt",
                   st.wYear,
                   st.wMonth,
                   st.wDay,
                   st.wHour,
                   st.wMinute,
                   st.wSecond,
                   GetCurrentProcessId(),
                   GetCurrentThreadId());

        std::filesystem::path folder = get_last_chance_folder();
        std::error_code ec;
        std::filesystem::create_directories(folder, ec);
        return folder / filenameBuffer;
    }

    static void write_memory_region_description(HANDLE file, const char* label, void* address) {
        write_last_chance_format(file, "%s: 0x%016llX\n", label, static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(address)));

        MEMORY_BASIC_INFORMATION mbi = {};
        if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
            write_last_chance_raw(file, "  VirtualQuery: failed\n");
            return;
        }

        write_last_chance_format(file,
                                 "  Region Base: 0x%016llX\n"
                                 "  Allocation Base: 0x%016llX\n"
                                 "  Region Size: 0x%llX\n"
                                 "  State: %s (0x%08lX)\n"
                                 "  Protect: %s (0x%08lX)\n"
                                 "  Allocation Protect: %s (0x%08lX)\n"
                                 "  Type: %s (0x%08lX)\n",
                                 static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(mbi.BaseAddress)),
                                 static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(mbi.AllocationBase)),
                                 static_cast<unsigned long long>(mbi.RegionSize),
                                 memory_state_name(mbi.State),
                                 mbi.State,
                                 memory_protect_name(mbi.Protect),
                                 mbi.Protect,
                                 memory_protect_name(mbi.AllocationProtect),
                                 mbi.AllocationProtect,
                                 memory_type_name(mbi.Type),
                                 mbi.Type);

        HMODULE module = nullptr;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               reinterpret_cast<LPCSTR>(address),
                               &module) && module != nullptr) {
            char moduleName[MAX_PATH] = {};
            GetModuleFileNameA(module, moduleName, MAX_PATH);
            const auto moduleBase = reinterpret_cast<uintptr_t>(module);
            const auto target = reinterpret_cast<uintptr_t>(address);
            write_last_chance_format(file,
                                     "  Module: %s\n"
                                     "  Module Base: 0x%016llX\n"
                                     "  Module RVA: 0x%llX\n",
                                     moduleName[0] == '\0' ? "(unnamed)" : moduleName,
                                     static_cast<unsigned long long>(moduleBase),
                                     static_cast<unsigned long long>(target - moduleBase));
        } else {
            write_last_chance_raw(file, "  Module: none\n");
        }
    }

    static void write_context_summary(HANDLE file, CONTEXT* ctx) {
        if (!ctx) {
            write_last_chance_raw(file, "Context: unavailable\n");
            return;
        }

#ifdef _WIN64
        write_last_chance_format(file, "RAX: 0x%016llX  RBX: 0x%016llX\n", static_cast<unsigned long long>(ctx->Rax), static_cast<unsigned long long>(ctx->Rbx));
        write_last_chance_format(file, "RCX: 0x%016llX  RDX: 0x%016llX\n", static_cast<unsigned long long>(ctx->Rcx), static_cast<unsigned long long>(ctx->Rdx));
        write_last_chance_format(file, "RSI: 0x%016llX  RDI: 0x%016llX\n", static_cast<unsigned long long>(ctx->Rsi), static_cast<unsigned long long>(ctx->Rdi));
        write_last_chance_format(file, "RBP: 0x%016llX  RSP: 0x%016llX\n", static_cast<unsigned long long>(ctx->Rbp), static_cast<unsigned long long>(ctx->Rsp));
        write_last_chance_format(file, "R8:  0x%016llX  R9:  0x%016llX\n", static_cast<unsigned long long>(ctx->R8), static_cast<unsigned long long>(ctx->R9));
        write_last_chance_format(file, "R10: 0x%016llX  R11: 0x%016llX\n", static_cast<unsigned long long>(ctx->R10), static_cast<unsigned long long>(ctx->R11));
        write_last_chance_format(file, "R12: 0x%016llX  R13: 0x%016llX\n", static_cast<unsigned long long>(ctx->R12), static_cast<unsigned long long>(ctx->R13));
        write_last_chance_format(file, "R14: 0x%016llX  R15: 0x%016llX\n", static_cast<unsigned long long>(ctx->R14), static_cast<unsigned long long>(ctx->R15));
        write_last_chance_format(file, "RIP: 0x%016llX  EFLAGS: 0x%016llX\n", static_cast<unsigned long long>(ctx->Rip), static_cast<unsigned long long>(ctx->EFlags));
#else
        write_last_chance_format(file, "EAX: 0x%08lX  EBX: 0x%08lX\n", ctx->Eax, ctx->Ebx);
        write_last_chance_format(file, "ECX: 0x%08lX  EDX: 0x%08lX\n", ctx->Ecx, ctx->Edx);
        write_last_chance_format(file, "ESI: 0x%08lX  EDI: 0x%08lX\n", ctx->Esi, ctx->Edi);
        write_last_chance_format(file, "EBP: 0x%08lX  ESP: 0x%08lX\n", ctx->Ebp, ctx->Esp);
        write_last_chance_format(file, "EIP: 0x%08lX  EFLAGS: 0x%08lX\n", ctx->Eip, ctx->EFlags);
#endif
    }

    static void write_pointer_module_suffix(HANDLE file, uintptr_t value) {
        HMODULE module = nullptr;
        if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                reinterpret_cast<LPCSTR>(value),
                                &module) || module == nullptr) {
            write_last_chance_raw(file, "\n");
            return;
        }

        char moduleName[MAX_PATH] = {};
        GetModuleFileNameA(module, moduleName, MAX_PATH);
        const char* fileName = std::strrchr(moduleName, '\\');
        fileName = fileName ? fileName + 1 : moduleName;
        const auto moduleBase = reinterpret_cast<uintptr_t>(module);
        write_last_chance_format(file,
                                 "  %s+0x%llX\n",
                                 fileName[0] == '\0' ? "(module)" : fileName,
                                 static_cast<unsigned long long>(value - moduleBase));
    }

    static void write_stack_summary(HANDLE file, CONTEXT* ctx) {
        if (!ctx) {
            write_last_chance_raw(file, "Stack: unavailable\n");
            return;
        }

#ifdef _WIN64
        const auto stackPointer = static_cast<uintptr_t>(ctx->Rsp);
#else
        const auto stackPointer = static_cast<uintptr_t>(ctx->Esp);
#endif
        write_last_chance_format(file, "Stack qwords from 0x%016llX:\n", static_cast<unsigned long long>(stackPointer));

        for (std::size_t i = 0; i < 48; ++i) {
            const auto address = stackPointer + i * sizeof(uintptr_t);
            uintptr_t value = 0;
            SIZE_T bytesRead = 0;
            if (!ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<const void*>(address), &value, sizeof(value), &bytesRead)
                || bytesRead != sizeof(value)) {
                write_last_chance_format(
                    file,
                    "  [rsp+0x%03llX] 0x%016llX  <unreadable>\n",
                    static_cast<unsigned long long>(i * sizeof(uintptr_t)),
                    static_cast<unsigned long long>(address)
                );
                continue;
            }

            write_last_chance_format(
                file,
                "  [rsp+0x%03llX] 0x%016llX -> 0x%016llX",
                static_cast<unsigned long long>(i * sizeof(uintptr_t)),
                static_cast<unsigned long long>(address),
                static_cast<unsigned long long>(value)
            );
            write_pointer_module_suffix(file, value);
        }
    }

    static void write_last_chance_crash_log(EXCEPTION_POINTERS* ex_ptrs, const char* source) {
        if (last_chance_crashlog_written.exchange(true)) {
            return;
        }

        std::filesystem::path path;
        try {
            path = make_last_chance_path();
            last_chance_crashlog_path = path.string();
        } catch (...) {
            last_chance_crashlog_path = "unavailable";
            return;
        }

        const std::wstring widePath = path.wstring();
        HANDLE file = CreateFileW(widePath.c_str(),
                                  GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_DELETE,
                                  nullptr,
                                  CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL,
                                  nullptr);
        if (file == INVALID_HANDLE_VALUE) {
            return;
        }

        SYSTEMTIME st;
        GetLocalTime(&st);
        write_last_chance_raw(file, "===============================================\n");
        write_last_chance_raw(file, "     FLARIAL LAST-CHANCE CRASH CAPTURE\n");
        write_last_chance_raw(file, "===============================================\n\n");
        write_last_chance_raw(file, "This file is written synchronously from the exception path before the full crash reporter runs.\n");
        write_last_chance_raw(file, "If no normal crash_*.txt exists, attach this file with latest.log/latest_*.log.\n\n");
        write_last_chance_format(file, "COMMIT_HASH: %s\n", COMMIT_HASH);
        write_last_chance_format(file,
                                 "Timestamp: %04u-%02u-%02u %02u:%02u:%02u.%03u\n"
                                 "Source: %s\n"
                                 "Process ID: %lu\n"
                                 "Thread ID: %lu\n"
                                 "Debugger Attached: %s\n\n",
                                 static_cast<unsigned int>(st.wYear),
                                 static_cast<unsigned int>(st.wMonth),
                                 static_cast<unsigned int>(st.wDay),
                                 static_cast<unsigned int>(st.wHour),
                                 static_cast<unsigned int>(st.wMinute),
                                 static_cast<unsigned int>(st.wSecond),
                                 static_cast<unsigned int>(st.wMilliseconds),
                                 source == nullptr ? "unknown" : source,
                                 GetCurrentProcessId(),
                                 crash_thread_id == 0 ? GetCurrentThreadId() : crash_thread_id,
                                 IsDebuggerPresent() ? "yes" : "no");

        if (ex_ptrs && ex_ptrs->ExceptionRecord) {
            EXCEPTION_RECORD* record = ex_ptrs->ExceptionRecord;
            write_last_chance_format(file,
                                     "Exception Code: 0x%08lX (%s)\n"
                                     "Exception Address: 0x%016llX\n"
                                     "Exception Flags: 0x%08lX\n"
                                     "Exception Parameters: %lu\n",
                                     record->ExceptionCode,
                                     get_exception_name(record->ExceptionCode).c_str(),
                                     static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(record->ExceptionAddress)),
                                     record->ExceptionFlags,
                                     record->NumberParameters);

            if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters >= 2) {
                write_last_chance_format(file,
                                         "Access Violation Operation: %s (%llu)\n"
                                         "Access Violation Target: 0x%016llX\n",
                                         access_violation_operation(record->ExceptionInformation[0]),
                                         static_cast<unsigned long long>(record->ExceptionInformation[0]),
                                         static_cast<unsigned long long>(record->ExceptionInformation[1]));
            }

            write_last_chance_raw(file, "\nException Address Region:\n");
            write_memory_region_description(file, "  Exception Address", record->ExceptionAddress);

            if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters >= 2) {
                write_last_chance_raw(file, "\nAccess Target Region:\n");
                write_memory_region_description(file, "  Access Target", reinterpret_cast<void*>(record->ExceptionInformation[1]));
            }
        } else {
            write_last_chance_raw(file, "Exception Record: unavailable\n");
        }

        write_last_chance_raw(file, "\nRegister Summary:\n");
        write_context_summary(file, ex_ptrs ? ex_ptrs->ContextRecord : nullptr);
        write_last_chance_raw(file, "\nStack Summary:\n");
        write_stack_summary(file, ex_ptrs ? ex_ptrs->ContextRecord : nullptr);
        CloseHandle(file);
    }

    // Backup latest.log to a timestamped file so it's not lost if user relaunches quickly
    static void backup_latest_log() {
        try {
            std::string latestLogPath = Utils::getClientPath() + "\\logs\\latest.log";

            if (!std::filesystem::exists(latestLogPath)) {
                return; // Nothing to backup
            }

            // Create backup filename with timestamp: latest_2024-01-15-14-30-45.log
            std::string timestamp = current_timestamp();
            std::string backupPath = Utils::getClientPath() + "\\logs\\latest_" + timestamp + ".log";

            // Copy the file (don't move - we want to preserve original for telemetry)
            std::filesystem::copy_file(latestLogPath, backupPath,
                std::filesystem::copy_options::overwrite_existing);
        } catch (const std::exception& e) {
            // Don't let backup errors affect crash handling
        }
    }

    //write a minidump file alongside the .txt log. Compact memory footprint:
    //thread info + stacks + memory referenced by registers, no full memory dump.
    static std::string write_minidump(EXCEPTION_POINTERS* ep, const std::string& timestamp) {
        std::error_code err;
        std::filesystem::create_directories(output_folder, err);

        std::filesystem::path dumpPath = output_folder / ("crash_" + timestamp + ".dmp");

        HANDLE hFile = CreateFileW(
            dumpPath.wstring().c_str(),
            GENERIC_WRITE, 0, nullptr,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) return "";

        MINIDUMP_EXCEPTION_INFORMATION mdei = {};
        mdei.ThreadId = (crash_thread_id != 0) ? crash_thread_id : GetCurrentThreadId();
        mdei.ExceptionPointers = ep;
        mdei.ClientPointers = FALSE;

        const MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(
            MiniDumpWithThreadInfo |
            MiniDumpWithIndirectlyReferencedMemory |
            MiniDumpScanMemory |
            MiniDumpWithUnloadedModules);

        BOOL ok = MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile, type,
            ep ? &mdei : nullptr,
            nullptr, nullptr);

        CloseHandle(hFile);
        return ok ? dumpPath.string() : "";
    }

    //output the crashlog file after a crash has occured
    static void output_crash_log() {
        // Backup latest.log first (before it gets overwritten on next launch)
        backup_latest_log();

        try {
            current_crash_id = CrashTelemetry::generateCrashId();
            current_report_id = CrashTelemetry::generateReportId();
        } catch (...) {
            current_crash_id = "unavailable";
            current_report_id = "unavailable";
        }

        // Generate the crash dump content
        crash_dump_content = generate_crash_dump();

        // Write to file
        std::filesystem::path path = get_log_filepath();
        std::ofstream logFile(path);
        logFile << crash_dump_content;
        logFile.close();
        last_crashlog_path = path.string();

        // Write a paired minidump using the same timestamp as the .txt filename.
        // path.stem() is "crash_YYYY-MM-DD-HH-MM-SS"; strip the "crash_" prefix.
        try {
            std::string stem = path.stem().string();
            std::string ts = (stem.rfind("crash_", 0) == 0) ? stem.substr(6) : current_timestamp();
            write_minidump(exception_pointers, ts);
        } catch (...) {
            // Minidump failure must not block the .txt or telemetry path.
        }

        // Send crash telemetry with the crash dump
        try {
            std::string signalName = try_get_signal_name(crash_signal);
            CrashTelemetry::sendCrashReport(
                trace,
                crash_signal,
                signalName,
                exception_pointers,
                crash_dump_content,
                current_crash_id,
                current_report_id
            );

#if defined(__DEBUG__)
            // Also export user actions to file for manual review (debug only)
            // UserActionLogger::exportToFile();
#endif
        } catch (const std::exception& e) {
            // Don't let telemetry errors affect crash handling
        }

        if(on_output_crashlog) on_output_crashlog(path.string());
    }

    //get the current timestamp as a string, for the crash log filename
    static std::string current_timestamp() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[80];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
        return buffer;
    }

    static std::string format_uptime() {
        ULONGLONG total_seconds = GetTickCount64() / 1000;
        ULONGLONG hours = total_seconds / 3600;
        ULONGLONG minutes = (total_seconds % 3600) / 60;
        ULONGLONG seconds = total_seconds % 60;

        std::stringstream ss;
        ss << hours << "h " << minutes << "m " << seconds << "s";
        return ss.str();
    }

    static int signal_from_exception_code(DWORD exception_code) {
        switch (exception_code) {
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
            case EXCEPTION_INT_OVERFLOW:
            case EXCEPTION_FLT_DENORMAL_OPERAND:
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            case EXCEPTION_FLT_INEXACT_RESULT:
            case EXCEPTION_FLT_INVALID_OPERATION:
            case EXCEPTION_FLT_OVERFLOW:
            case EXCEPTION_FLT_STACK_CHECK:
            case EXCEPTION_FLT_UNDERFLOW:
                return SIGFPE;
            case EXCEPTION_ILLEGAL_INSTRUCTION:
            case EXCEPTION_PRIV_INSTRUCTION:
                return SIGILL;
            case EXCEPTION_ACCESS_VIOLATION:
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_STACK_OVERFLOW:
                return SIGSEGV;
            default:
                return SIGABRT;
        }
    }

    static bool is_vectored_fatal_exception(DWORD exception_code) {
        switch (exception_code) {
            case STATUS_FAIL_FAST_EXCEPTION:
            case STATUS_HEAP_CORRUPTION:
            case STATUS_STACK_BUFFER_OVERRUN:
            case STATUS_FATAL_APP_EXIT:
            case EXCEPTION_STACK_OVERFLOW:
                return true;
            default:
                return false;
        }
    }

    static bool is_execute_access_violation(EXCEPTION_POINTERS* ex_ptrs) {
        if (!ex_ptrs || !ex_ptrs->ExceptionRecord) {
            return false;
        }

        EXCEPTION_RECORD* record = ex_ptrs->ExceptionRecord;
        return record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION
            && record->NumberParameters >= 2
            && record->ExceptionInformation[0] == 8;
    }

    static bool should_capture_vectored_exception(DWORD exception_code, EXCEPTION_POINTERS* ex_ptrs) {
        if (is_vectored_fatal_exception(exception_code)) {
            return true;
        }

        switch (exception_code) {
            case EXCEPTION_ACCESS_VIOLATION:
                // First-chance read/write AVs can be deliberate SEH probes. DEP/execute AVs are not.
                return is_execute_access_violation(ex_ptrs);
            case EXCEPTION_ILLEGAL_INSTRUCTION:
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_INVALID_DISPOSITION:
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            case EXCEPTION_PRIV_INSTRUCTION:
                return true;
            default:
                return false;
        }
    }

    //utility function needed for crash log timestamps (replace {timestamp} in filename format with the timestamp string)
    static std::string replace_substr(std::string str, const std::string& search, const std::string& replace) {
        if(search.empty()) return str;
        size_t pos = 0;
        while((pos = str.find(search, pos)) != std::string::npos) {
            str.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return str;
    }

    //get the crash log filename
    std::filesystem::path get_log_filepath() {
        std::string timestampstr = current_timestamp();
        std::string timestampped_filename = replace_substr(filename, "{timestamp}", timestampstr);

        std::filesystem::path filepath = output_folder / timestampped_filename;

        //ensure the crash log folder exists. error code is here to supporess errors... since we're in an error handler already
        //at default settings this errors because an empty path is specified... lol. but we dont want to create a folder in that case anyway
        std::error_code err;
        std::filesystem::create_directories(output_folder, err);

        return filepath;
    }

    //using a thread here is a hack to get stack space in the case where the crash is a stack overflow
    //this hack was borrowed from backward.cpp
    static void crash_handler_thread() {
        //wait for the program to crash or exit normally
        std::unique_lock<std::mutex> lk(mut);
        cv.wait(lk, [] { return status != program_status::running; });
        lk.unlock();

        if(status == program_status::end_session)
            return;

        //if it crashed, output the crash log
        if(status == program_status::crashed) {
            output_crash_log();

#ifndef __DEBUG__
            std::wstringstream ss;
            ss << L"Flarial crashed and saved a crash report.\n\n";
            ss << L"Crash ID: " << utf8_to_wide(current_crash_id.empty() ? "unavailable" : current_crash_id) << L"\n";
            ss << L"Report ID: " << utf8_to_wide(current_report_id.empty() ? "unavailable" : current_report_id) << L"\n";
            ss << L"Game Version: " << utf8_to_wide(Client::version.empty() ? "unavailable" : Client::version) << L"\n";
            ss << L"Commit: " << utf8_to_wide(COMMIT_HASH) << L"\n";

            if (crash_signal != 0) {
                ss << L"Signal: " << crash_signal;
                const std::string signalName = try_get_signal_name(crash_signal);
                if (!signalName.empty()) {
                    ss << L" (" << utf8_to_wide(signalName) << L")";
                }
                ss << L"\n";
            }

            if (exception_pointers && exception_pointers->ExceptionRecord) {
                const DWORD code = exception_pointers->ExceptionRecord->ExceptionCode;
                ss << L"Exception: " << utf8_to_wide(get_exception_name(code)) << L" (" << utf8_to_wide(format_hex(code)) << L")\n";
            }

            if (!last_crashlog_path.empty()) {
                const std::string crashlogFileName = std::filesystem::path(last_crashlog_path).filename().string();
                ss << L"Crashlog: " << utf8_to_wide(crashlogFileName) << L"\n";
            }
            if (!last_chance_crashlog_path.empty()) {
                const std::string crashlogFileName = std::filesystem::path(last_chance_crashlog_path).filename().string();
                ss << L"Last-chance: " << utf8_to_wide(crashlogFileName) << L"\n";
            }

            ss << L"\nPlease screenshot this dialog when asking for help.\n";
            ss << L"Open the logs folder now?";

            const std::wstring fullMsg = ss.str();
            int result = ShellMessageUtil::showW(nullptr, fullMsg.c_str(), L"Client Crashed - Please report this!", MB_YESNO | MB_ICONERROR);
            if (result == IDYES) {
                std::wstring logsPath(output_folder.wstring());
                ShellExecuteW(nullptr, L"open", logsPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            }
#endif
        }

        //alert the crashing thread we're done with the crash log so it can finish crashing
        status = program_status::ending;
        cv.notify_one();
    }

    static inline void crash_handler() {
        if(status == program_status::end_session)
            return;
        //if we crashed during a crash... ignore lol
        if(status != program_status::running) return;

        crash_thread_id = GetCurrentThreadId();
        write_last_chance_crash_log(exception_pointers, "crash_handler");

        //save the stacktrace
        try {
            trace = std::stacktrace::current();
        } catch (...) {
            trace = std::stacktrace();
        }

        // If the writer thread hasn't been spawned yet (crash before
        // begin_monitoring(), e.g. early init), the cv-based
        // handoff would block forever. Fall back to a synchronous write on this
        // thread so we still produce a log + minidump.
        if (!output_thread.joinable()) {
            status = program_status::crashed;
            try {
                output_crash_log();
            } catch (...) {}
            status = program_status::ending;
            return;
        }

        //resume the monitoring thread
        status = program_status::crashed;
        cv.notify_one();

        //wait for the crash log to finish writing
        std::unique_lock<std::mutex> lk(mut);
        if (!cv.wait_for(lk, std::chrono::seconds(10), [] { return status != program_status::crashed; })) {
            status = program_status::ending;
        }
    }

    //Try to get the string representation of a signal identifier, return an empty string if none is found.
    //This only covers the signals from the C++ std lib and none of the POSIX or OS specific signal names!
    static const char* try_get_signal_name(int signal) {
        switch (signal) {
            case SIGTERM:
                return "SIGTERM";
            case SIGSEGV:
                return "SIGSEGV";
            case SIGINT:
                return "SIGINT";
            case SIGILL:
                return "SIGILL";
            case SIGABRT:
                return "SIGABRT";
            case SIGFPE:
                return "SIGFPE";
        }
        return "";
    }

    //Get human-readable exception name
    static std::string get_exception_name(DWORD code) {
        switch (code) {
            case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
            case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
            case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
            case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
            case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
            case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
            case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
            case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
            case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
            case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
            default: return "UNKNOWN_EXCEPTION";
        }
    }

    //Format hex value
    static std::string format_hex(DWORD64 value) {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(16) << std::setfill('0') << value;
        return ss.str();
    }

    //Get exception information
    static std::string get_exception_info() {
        if (!exception_pointers || !exception_pointers->ExceptionRecord) {
            return "No exception record available";
        }

        std::stringstream ss;
        auto* record = exception_pointers->ExceptionRecord;

        ss << "Exception Code: " << format_hex(record->ExceptionCode)
           << " (" << get_exception_name(record->ExceptionCode) << ")\n";
        ss << "Exception Address: " << format_hex((DWORD64)record->ExceptionAddress) << "\n";
        ss << "Exception Flags: " << format_hex(record->ExceptionFlags);

        if (record->ExceptionFlags & EXCEPTION_NONCONTINUABLE) {
            ss << " (NONCONTINUABLE)";
        }
        ss << "\n";

        // Additional info for access violations
        if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters >= 2) {
            ss << "\nAccess Violation Details:\n";
            ss << "  Type: " << (record->ExceptionInformation[0] == 0 ? "Read" :
                                  record->ExceptionInformation[0] == 1 ? "Write" : "Execute") << "\n";
            ss << "  Address: " << format_hex(record->ExceptionInformation[1]) << "\n";
        }

        return ss.str();
    }

    //Get register dump
    static std::string get_register_dump(CONTEXT* ctx) {
        if (!ctx) return "No context available";

        std::stringstream ss;

#ifdef _WIN64
        ss << "RAX: " << format_hex(ctx->Rax) << "  RBX: " << format_hex(ctx->Rbx) << "\n";
        ss << "RCX: " << format_hex(ctx->Rcx) << "  RDX: " << format_hex(ctx->Rdx) << "\n";
        ss << "RSI: " << format_hex(ctx->Rsi) << "  RDI: " << format_hex(ctx->Rdi) << "\n";
        ss << "RBP: " << format_hex(ctx->Rbp) << "  RSP: " << format_hex(ctx->Rsp) << "\n";
        ss << "R8:  " << format_hex(ctx->R8)  << "  R9:  " << format_hex(ctx->R9) << "\n";
        ss << "R10: " << format_hex(ctx->R10) << "  R11: " << format_hex(ctx->R11) << "\n";
        ss << "R12: " << format_hex(ctx->R12) << "  R13: " << format_hex(ctx->R13) << "\n";
        ss << "R14: " << format_hex(ctx->R14) << "  R15: " << format_hex(ctx->R15) << "\n";
        ss << "RIP: " << format_hex(ctx->Rip) << "\n";
        ss << "EFLAGS: " << format_hex(ctx->EFlags) << "\n";
#else
        ss << "EAX: " << format_hex(ctx->Eax) << "  EBX: " << format_hex(ctx->Ebx) << "\n";
        ss << "ECX: " << format_hex(ctx->Ecx) << "  EDX: " << format_hex(ctx->Edx) << "\n";
        ss << "ESI: " << format_hex(ctx->Esi) << "  EDI: " << format_hex(ctx->Edi) << "\n";
        ss << "EBP: " << format_hex(ctx->Ebp) << "  ESP: " << format_hex(ctx->Esp) << "\n";
        ss << "EIP: " << format_hex(ctx->Eip) << "\n";
        ss << "EFLAGS: " << format_hex(ctx->EFlags) << "\n";
#endif

        return ss.str();
    }

    //Get loaded modules information
    static std::string get_loaded_modules() {
        std::stringstream ss;
        HANDLE process = GetCurrentProcess();
        HMODULE modules[1024];
        DWORD needed;

        if (EnumProcessModules(process, modules, sizeof(modules), &needed)) {
            size_t module_count = needed / sizeof(HMODULE);

            for (size_t i = 0; i < module_count; i++) {
                char module_name[MAX_PATH];
                MODULEINFO mod_info;

                if (GetModuleFileNameExA(process, modules[i], module_name, sizeof(module_name))) {
                    if (GetModuleInformation(process, modules[i], &mod_info, sizeof(mod_info))) {
                        ss << format_hex((DWORD64)mod_info.lpBaseOfDll) << " - "
                           << format_hex((DWORD64)mod_info.lpBaseOfDll + mod_info.SizeOfImage)
                           << "  " << module_name << "\n";
                    }
                }
            }
        } else {
            ss << "Failed to enumerate modules\n";
        }

        return ss.str();
    }

    //Get memory dump around crash address
    static std::string get_memory_dump(void* address) {
        std::stringstream ss;

        if (!address) {
            return "Invalid address (NULL)";
        }

        ss << "Memory dump at " << format_hex((DWORD64)address) << ":\n\n";

        // Try to read 128 bytes before and after the crash address
        const size_t dump_size = 128;
        BYTE* base_addr = (BYTE*)address - dump_size;

        for (int offset = -((int)dump_size); offset < (int)dump_size; offset += 16) {
            BYTE* addr = (BYTE*)address + offset;
            ss << format_hex((DWORD64)addr) << ": ";

            // Try to read 16 bytes
            BYTE buffer[16];
            SIZE_T bytes_read = 0;
            bool readable = ReadProcessMemory(GetCurrentProcess(), addr, buffer, 16, &bytes_read) != 0;

            if (readable && bytes_read > 0) {
                // Hex dump
                for (size_t i = 0; i < 16; i++) {
                    if (i < bytes_read) {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i] << " ";
                    } else {
                        ss << "?? ";
                    }
                }

                ss << " | ";

                // ASCII representation
                for (size_t i = 0; i < bytes_read; i++) {
                    char c = buffer[i];
                    ss << (isprint(c) ? c : '.');
                }
            } else {
                ss << "?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? | ????????????????";
            }

            // Mark the crash address line
            if (offset == 0) {
                ss << "  <-- CRASH HERE";
            }

            ss << "\n";
        }

        return ss.str();
    }

    //Get stack trace for a specific thread using DbgHelp
    static std::string walk_thread_stack(HANDLE hThread, CONTEXT* ctx) {
        std::stringstream ss;
        HANDLE process = GetCurrentProcess();

        // Initialize symbol handler
        static bool symbolsInitialized = false;
        if (!symbolsInitialized) {
            SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
            SymInitialize(process, NULL, TRUE);
            symbolsInitialized = true;
        }

        STACKFRAME64 stackFrame = {};
#ifdef _WIN64
        DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
        stackFrame.AddrPC.Offset = ctx->Rip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = ctx->Rbp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = ctx->Rsp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#else
        DWORD machineType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset = ctx->Eip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = ctx->Ebp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = ctx->Esp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

        int frameNum = 0;
        const int maxFrames = 64;

        while (frameNum < maxFrames) {
            if (!StackWalk64(machineType, process, hThread, &stackFrame, ctx,
                            NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
                break;
            }

            if (stackFrame.AddrPC.Offset == 0) {
                break;
            }

            ss << "    frame #" << frameNum << ": " << format_hex(stackFrame.AddrPC.Offset);

            // Get module name
            HMODULE hModule = NULL;
            char moduleName[MAX_PATH] = "";
            GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                              (LPCSTR)stackFrame.AddrPC.Offset, &hModule);
            if (hModule) {
                GetModuleFileNameExA(process, hModule, moduleName, MAX_PATH);
                // Extract just the filename
                char* lastSlash = strrchr(moduleName, '\\');
                ss << " " << (lastSlash ? lastSlash + 1 : moduleName);
            }

            // Get symbol name
            char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen = MAX_SYM_NAME;

            DWORD64 displacement = 0;
            if (SymFromAddr(process, stackFrame.AddrPC.Offset, &displacement, pSymbol)) {
                ss << "`" << pSymbol->Name;
                if (displacement > 0) {
                    ss << " + 0x" << std::hex << displacement;
                }
            }

            // Get source file and line
            IMAGEHLP_LINE64 line = {};
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD lineDisplacement = 0;
            if (SymGetLineFromAddr64(process, stackFrame.AddrPC.Offset, &lineDisplacement, &line)) {
                ss << " at " << line.FileName << ":" << std::dec << line.LineNumber;
            }

            ss << "\n";
            frameNum++;
        }

        return ss.str();
    }

    //Get stack traces for all threads (bt all equivalent)
    static std::string get_all_thread_stacks() {
        std::stringstream ss;
        DWORD currentProcessId = GetCurrentProcessId();
        DWORD handlerThreadId = GetCurrentThreadId();
        DWORD targetCrashThreadId = crash_thread_id != 0 ? crash_thread_id : handlerThreadId;

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return "Failed to create thread snapshot\n";
        }

        THREADENTRY32 te;
        te.dwSize = sizeof(THREADENTRY32);

        if (!Thread32First(hSnapshot, &te)) {
            CloseHandle(hSnapshot);
            return "Failed to enumerate threads\n";
        }

        int threadNum = 0;
        do {
            if (te.th32OwnerProcessID != currentProcessId) {
                continue;
            }

            ss << "  thread #" << threadNum++;
            const bool isCrashThread = te.th32ThreadID == targetCrashThreadId;
            const bool isHandlerThread = te.th32ThreadID == handlerThreadId;
            if (isCrashThread) {
                ss << " (CRASHING THREAD)";
            }
            ss << ", tid = " << te.th32ThreadID << "\n";

            HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION,
                                        FALSE, te.th32ThreadID);
            if (hThread != NULL) {
                bool needsResume = false;
                if (!isCrashThread && !isHandlerThread) {
                    if (SuspendThread(hThread) != (DWORD)-1) {
                        needsResume = true;
                    }
                }

                CONTEXT ctx = {};
                ctx.ContextFlags = CONTEXT_FULL;

                // For crashing thread, use exception context if available
                if (isCrashThread && exception_pointers && exception_pointers->ContextRecord) {
                    ctx = *exception_pointers->ContextRecord;
                    ss << walk_thread_stack(hThread, &ctx);
                } else if (isCrashThread) {
                    ss << "    (crashing thread context unavailable)\n";
                } else if (isHandlerThread) {
                    RtlCaptureContext(&ctx);
                    ss << walk_thread_stack(GetCurrentThread(), &ctx);
                } else if (GetThreadContext(hThread, &ctx)) {
                    ss << walk_thread_stack(hThread, &ctx);
                } else {
                    ss << "    (unable to get thread context)\n";
                }

                if (needsResume) {
                    ResumeThread(hThread);
                }
                CloseHandle(hThread);
            } else {
                ss << "    (unable to open thread)\n";
            }

        } while (Thread32Next(hSnapshot, &te));

        CloseHandle(hSnapshot);
        return ss.str();
    }

    //various callbacks needed to get into the crash handler during a crash (borrowed from backward.cpp)
    static inline void signal_handler(int signal) {
        crash_signal = signal;
        crash_handler();
        std::quick_exit(1);
    }
    static inline void terminator() {
        if (crash_signal == 0) {
            crash_signal = SIGABRT;
        }
        crash_handler();
        std::quick_exit(1);
    }
    __declspec(noinline) static LONG CALLBACK vectored_handler(EXCEPTION_POINTERS* ex_ptrs) {
        if (!ex_ptrs || !ex_ptrs->ExceptionRecord) {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        const DWORD exceptionCode = ex_ptrs->ExceptionRecord->ExceptionCode;
        if (!should_capture_vectored_exception(exceptionCode, ex_ptrs)) {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        exception_pointers = ex_ptrs;
        if (crash_signal == 0) {
            crash_signal = signal_from_exception_code(exceptionCode);
        }
        crash_thread_id = GetCurrentThreadId();
        write_last_chance_crash_log(ex_ptrs, "vectored");
        crash_handler();
        return EXCEPTION_CONTINUE_SEARCH;
    }
    __declspec(noinline) static LONG WINAPI exception_handler(EXCEPTION_POINTERS* ex_ptrs) {
        exception_pointers = ex_ptrs;
        if (ex_ptrs && ex_ptrs->ExceptionRecord && crash_signal == 0) {
            crash_signal = signal_from_exception_code(ex_ptrs->ExceptionRecord->ExceptionCode);
        }
        crash_thread_id = GetCurrentThreadId();
        write_last_chance_crash_log(ex_ptrs, "unhandled_exception_filter");
        crash_handler();
        return EXCEPTION_CONTINUE_SEARCH;
    }
    static void __cdecl invalid_parameter_handler(const wchar_t*,const wchar_t*,const wchar_t*,unsigned int,uintptr_t) {
        crash_signal = SIGABRT;
        crash_handler();
        abort();
    }

    //callback needed during a normal exit to shut down the thread
    static inline void normal_exit() {
        status = program_status::normal_exit;
        cv.notify_one();
        if(output_thread.joinable())
            output_thread.join();
    }

    void end_session() {
        try {
            SetUnhandledExceptionFilter(nullptr);
            std::signal(SIGABRT, nullptr);
            std::signal(SIGSEGV, nullptr);
            std::signal(SIGILL, nullptr);
            std::signal(SIGFPE, nullptr);
            std::set_terminate(nullptr);
            _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
            _set_purecall_handler(nullptr);
            _set_invalid_parameter_handler(nullptr);
            if (vectored_exception_handle != nullptr) {
                RemoveVectoredExceptionHandler(vectored_exception_handle);
                vectored_exception_handle = nullptr;
            }

            status = program_status::end_session;
            cv.notify_all();

            if (output_thread.joinable()) {
                output_thread.join();
            }
        } catch (...) {
            std::cerr << "Exception occurred during end_session" << std::endl;
            throw;
        }
    }

    // Idempotent registration of all OS-level exception filters.
    // Keep this outside DllMain: it touches CRT signal/terminate hooks and global state.
    void early_register() {
        static std::atomic<bool> registered{false};
        bool expected = false;
        if (!registered.compare_exchange_strong(expected, true)) return;

        if (output_folder.empty()) {
            output_folder = resolve_fallback_folder();
        }

        SetUnhandledExceptionFilter(exception_handler);
        vectored_exception_handle = AddVectoredExceptionHandler(1, vectored_handler);
        std::signal(SIGABRT, signal_handler);
        std::signal(SIGSEGV, signal_handler);
        std::signal(SIGILL, signal_handler);
        std::signal(SIGFPE, signal_handler);
        std::set_terminate(terminator);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
        _set_purecall_handler(terminator);
        _set_invalid_parameter_handler(&invalid_parameter_handler);

        ULONG stackGuarantee = 128 * 1024;
        SetThreadStackGuarantee(&stackGuarantee);
    }

    //set up all the callbacks needed to get into the crash handler during a crash (borrowed from backward.cpp)
    void begin_monitoring() {
        // Initialize crash telemetry and user action logging
        CrashTelemetry::initialize();
        UserActionLogger::initialize();

        last_chance_crashlog_written = false;
        last_chance_crashlog_path.clear();
        last_crashlog_path.clear();
        crash_dump_content.clear();
        crash_thread_id = 0;
        crash_signal = 0;
        exception_pointers = nullptr;
        status = program_status::running;

        output_thread = std::thread(crash_handler_thread);

        // Filters may already be registered from the init thread prologue; early_register is idempotent.
        early_register();

        std::atexit(normal_exit);
    }
}
