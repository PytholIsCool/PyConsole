#pragma once
#include <string>
#include "ConsoleTypes.h"

namespace PyConsole {

    class Console final {
    public:
        // Global flags
        static bool isDebugMode;

        // Output
        static void Write(const std::string& text);
        static void Write(const std::string& text, COLORREF color);
        static void WriteLine(const std::string& text);
        static void WriteLine(const std::string& text, COLORREF color);

        // Input
        static std::string ReadLine();
        static char Read();    // getchar-style (buffered)
        static char ReadKey(); // immediate char

        // Color
        static void SetForegroundColor(COLORREF color);
        static void SetBackgroundColor(COLORREF color);
        static void ResetColor();

        // Cursor / Screen
        static void SetCursorPosition(int x, int y);
        static void Clear();

        // Styled diagnostics
        static void Debug(const std::string& text);
        static void Error(const std::string& text);

        // Window
        static void SetTitle(const std::string& title);

        // Cursor Info
        static int GetCursorX();
        static int GetCursorY();

        // Window Info
        static int GetWidth();
        static int GetHeight();

        // Input availability
        static bool KeyAvailable();

        // Utility
        static void Pause();

    private:
        // init + low-level helpers
        static void EnsureSetup();
        static void WriteRGBText(COLORREF color, const std::string& text);

        // State
        static bool s_isSetUp;
        static COLORREF s_foregroundColor;
        static COLORREF s_backgroundColor;

        // Platform handles and helpers (implemented in Console.Win32.cpp)
        static void PlatformSetup();
        static void PlatformClear();
        static void PlatformSetCursorPosition(int x, int y);
        static int  PlatformGetCursorX();
        static int  PlatformGetCursorY();
        static int  PlatformGetWidth();
        static int  PlatformGetHeight();
        static void PlatformSetTitle(const std::string& title);
        static bool PlatformKeyAvailable();
        static char PlatformReadKey();
    };

}