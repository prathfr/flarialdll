#include "ShellMessageUtil.hpp"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// ShellMessageBoxW provides themed dialogs (modern look), but it runs the lpcText
// parameter through wvsprintf internally. Instead of fighting format specifier
// semantics (%s vs %ls varies), we pass text directly as the format string with
// no variadic args. Any literal '%' in the text is escaped to '%%' to avoid
// wvsprintf interpreting it.

int ShellMessageUtil::showW(HWND hwnd, const wchar_t* text, const wchar_t* caption, UINT type) {
    HMODULE shlwapi = GetModuleHandleW(L"shlwapi.dll");
    if (!shlwapi) shlwapi = LoadLibraryW(L"shlwapi.dll");

    if (shlwapi) {
        using ShellMessageBoxW_t = int (WINAPIV*)(HINSTANCE, HWND, LPCWSTR, LPCWSTR, UINT, ...);
        auto pShellMessageBoxW = reinterpret_cast<ShellMessageBoxW_t>(
            GetProcAddress(shlwapi, "ShellMessageBoxW")
        );
        if (pShellMessageBoxW) {
            // Escape any '%' → '%%' so wvsprintf treats text as literal
            std::wstring escaped;
            for (const wchar_t* p = text; *p; ++p) {
                if (*p == L'%') escaped += L"%%";
                else escaped += *p;
            }
            return pShellMessageBoxW(nullptr, hwnd, escaped.c_str(), caption, type);
        }
    }

    return MessageBoxW(hwnd, text, caption, type);
}

int ShellMessageUtil::showA(HWND hwnd, const char* text, const char* caption, UINT type) {
    // Convert to wide and delegate to showW
    int textLen = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    int capLen = MultiByteToWideChar(CP_UTF8, 0, caption, -1, nullptr, 0);

    std::wstring wText(textLen, L'\0');
    std::wstring wCaption(capLen, L'\0');

    MultiByteToWideChar(CP_UTF8, 0, text, -1, wText.data(), textLen);
    MultiByteToWideChar(CP_UTF8, 0, caption, -1, wCaption.data(), capLen);

    return showW(hwnd, wText.c_str(), wCaption.c_str(), type);
}
