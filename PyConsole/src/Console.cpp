#include <iostream>
#include <string>
#include "../include/Console.h"

namespace PyConsole {
    bool     Console::s_isSetUp = false;
    bool     Console::isDebugMode = false;
    COLORREF Console::s_foregroundColor = RGB(255, 255, 255);
    COLORREF Console::s_backgroundColor = RGB(0, 0, 0);

    // Public

    void Console::Write(const std::string& text) {
        EnsureSetup();
        WriteRGBText(s_foregroundColor, text);
    }

    void Console::Write(const std::string& text, COLORREF color) {
        EnsureSetup();
        WriteRGBText(color, text);
    }

    void Console::WriteLine(const std::string& text) {
        Write(text);
        std::cout << "\n";
    }

    void Console::WriteLine(const std::string& text, COLORREF color) {
        Write(text, color);
        std::cout << "\n";
    }

    std::string Console::ReadLine() {
        EnsureSetup();
        std::string out;
        std::getline(std::cin, out);
        return out;
    }

    char Console::Read() {
        EnsureSetup();
        return static_cast<char>(std::cin.get());
    }

    char Console::ReadKey() {
        EnsureSetup();
        return PlatformReadKey();
    }

    void Console::SetForegroundColor(COLORREF color) { s_foregroundColor = color; }
    void Console::SetBackgroundColor(COLORREF color) { s_backgroundColor = color; }
    void Console::ResetColor() {
        s_foregroundColor = RGB(255, 255, 255);
        s_backgroundColor = RGB(0, 0, 0);
    }

    void Console::SetCursorPosition(int x, int y) {
        EnsureSetup();
        PlatformSetCursorPosition(x, y);
    }

    void Console::Clear() {
        EnsureSetup();
        PlatformClear();
    }

    void Console::Debug(const std::string& text) {
        if (!isDebugMode) return;
        Write("[DEBUG] ", RGB(255, 255, 224));
        WriteLine(text, RGB(255, 255, 224));
    }

    void Console::Error(const std::string& text) {
        Write("[ERROR] ", RGB(200, 50, 50));
        WriteLine(text, RGB(200, 50, 50));
    }

    void Console::SetTitle(const std::string& title) {
        EnsureSetup();
        PlatformSetTitle(title);
    }

    int Console::GetCursorX() { EnsureSetup(); return PlatformGetCursorX(); }
    int Console::GetCursorY() { EnsureSetup(); return PlatformGetCursorY(); }
    int Console::GetWidth() { EnsureSetup(); return PlatformGetWidth(); }
    int Console::GetHeight() { EnsureSetup(); return PlatformGetHeight(); }
    bool Console::KeyAvailable() { EnsureSetup(); return PlatformKeyAvailable(); }

    void Console::Pause() {
        WriteLine("Press ENTER to continue...", RGB(192, 192, 192));
        (void)ReadLine();
    }

    // Private

    void Console::EnsureSetup() {
        if (s_isSetUp) return;
        PlatformSetup();
        s_isSetUp = true;
    }

    // ANSI 24-bit color write (works on Win10+ with VT enabled)
    void Console::WriteRGBText(COLORREF color, const std::string& text) {
        std::cout << char(27) << "[38;2;"
            << (int)GetRValue(color) << ";"
            << (int)GetGValue(color) << ";"
            << (int)GetBValue(color) << "m"
            << text
            << char(27) << "[0m";
    }

}
