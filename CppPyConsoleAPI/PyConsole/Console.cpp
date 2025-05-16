#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "Console.hpp"

using Color = Console::Color;

HANDLE _cHan = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO _csbi;
COORD sC = { 0, 0 };
std::string output;

#pragma region ColorHandler

Color prevColor = Console::Color::White;

void SilentSet(Console::Color color) {
    SetConsoleTextAttribute(_cHan, static_cast<WORD>(color));
}

void SetEval(Color color) {
    if (color != Color::Placeholder)
        SilentSet(color);
    else
        SilentSet(prevColor);
}

void Console::SetForegroundColor(Color color) {
    prevColor = color;
    SilentSet(color);
}

void Console::SetBackgroundColor(Color color) { } // Won't do anything. I don't intend for this to work.

void Console::ResetColor() {
    SilentSet(Console::Color::White);
    prevColor = Console::Color::White;
}

#pragma endregion

#pragma region WriteUtils

void Console::Write(const std::string& text, Color color) {
    SetEval(color);
    std::cout << text;
    SilentSet(prevColor);
}

void Console::WriteLine(const std::string& text, Color color) {
    Write(text, color);
    std::cout << "\n";
}

void Console::WriteLine(Color color) {
    SetEval(color);
    std::cout << "\n";
    SilentSet(prevColor);
}

void Console::Debug(const std::string& text) {
    SilentSet(Color::LightYellow);
    std::cout << "[DEBUG] " << text << "\n";
    SilentSet(prevColor);
}

void Console::Error(const std::string& text) {
    SilentSet(Color::Red);
    std::cerr << "[ERROR] " << text << "\n";
    SilentSet(prevColor);
}

void Console::Clear() {
    GetConsoleScreenBufferInfo(_cHan, &_csbi);
    DWORD count, cellCount = _csbi.dwSize.X * _csbi.dwSize.Y;
    FillConsoleOutputCharacter(_cHan, ' ', cellCount, sC, &count);
    FillConsoleOutputAttribute(_cHan, _csbi.wAttributes, cellCount, sC, &count);
    SetConsoleCursorPosition(_cHan, sC);
}

#pragma endregion

#pragma region InputUtils

std::string Console::ReadLine() {
    std::getline(std::cin, output);
    return output;
}

char Console::Read() {
    return static_cast<char>(std::cin.get());
}

char Console::ReadKey() {
    return static_cast<char>(_getch()); // <conio.h> required
}

#pragma endregion

#pragma region CursorAndTitle

void Console::SetCursorPosition(int x, int y) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(_cHan, &info))
        return;

    if (x < 0 || x >= info.dwSize.X || y < 0 || y >= info.dwSize.Y)
        return;

    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(_cHan, pos);
}

void Console::SetTitle(const std::string& title) {
    SetConsoleTitleA(title.c_str());
}

#pragma endregion

#pragma region InfoUtils

int Console::GetCursorX() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(_cHan, &csbi))
        return csbi.dwCursorPosition.X;
    return -1;
}

int Console::GetCursorY() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(_cHan, &csbi))
        return csbi.dwCursorPosition.Y;
    return -1;
}

int Console::GetWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(_cHan, &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return -1;
}

int Console::GetHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(_cHan, &csbi))
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return -1;
}

bool Console::KeyAvailable() {
    return _kbhit();
}

void Console::Pause() {
    WriteLine("Press ENTER to continue...", Color::Gray);
    (void)ReadLine();
}

#pragma endregion