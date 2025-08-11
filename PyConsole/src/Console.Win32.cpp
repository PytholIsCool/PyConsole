#include <string>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "../include/Console.h"

namespace PyConsole {

    static HANDLE s_out = GetStdHandle(STD_OUTPUT_HANDLE);

    void Console::PlatformSetup() {
        // Enable ANSI VT processing
        DWORD mode = 0;
        if (GetConsoleMode(s_out, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(s_out, mode);
        }
        
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }

    void Console::PlatformClear() {
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (!GetConsoleScreenBufferInfo(s_out, &csbi)) return;

        const DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        DWORD count = 0;
        COORD home{ 0, 0 };

        FillConsoleOutputCharacterA(s_out, ' ', cellCount, home, &count);
        FillConsoleOutputAttribute(s_out, csbi.wAttributes, cellCount, home, &count);
        SetConsoleCursorPosition(s_out, home);
    }

    void Console::PlatformSetCursorPosition(int x, int y) {
        CONSOLE_SCREEN_BUFFER_INFO info{};
        if (!GetConsoleScreenBufferInfo(s_out, &info)) return;
        if (x < 0 || x >= info.dwSize.X || y < 0 || y >= info.dwSize.Y) return;
        COORD pos{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(s_out, pos);
    }

    int Console::PlatformGetCursorX() {
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (GetConsoleScreenBufferInfo(s_out, &csbi)) return csbi.dwCursorPosition.X;
        return -1;
    }
    int Console::PlatformGetCursorY() {
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (GetConsoleScreenBufferInfo(s_out, &csbi)) return csbi.dwCursorPosition.Y;
        return -1;
    }

    int Console::PlatformGetWidth() {
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (GetConsoleScreenBufferInfo(s_out, &csbi))
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        return -1;
    }
    int Console::PlatformGetHeight() {
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (GetConsoleScreenBufferInfo(s_out, &csbi))
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return -1;
    }

    void Console::PlatformSetTitle(const std::string& title) {
        SetConsoleTitleA(title.c_str());
    }

    bool Console::PlatformKeyAvailable() {
        return _kbhit() != 0;
    }

    char Console::PlatformReadKey() {
        return static_cast<char>(_getch());
    }

}