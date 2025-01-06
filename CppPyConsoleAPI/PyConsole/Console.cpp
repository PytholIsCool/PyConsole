#include <iostream>
#include <string>
#include <Windows.h>
#include "Console.hpp"
using Color = Console::Color;

HANDLE _cHan = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO _csbi;
COORD sC = { 0, 0 };
std::string output;

#pragma region ColorHandler
Color prevColor = Console::Color::White;

void SilentSet(Console::Color color) {
    SetConsoleTextAttribute(_cHan, static_cast<int>(color));
}
void SetEval(Color color) {
    if (color != Color::Placeholder)
        SilentSet(color);
    else if (prevColor == Color::White)
        SilentSet(Color::White);
    else
        SilentSet(prevColor);
}

void Console::SetForegroundColor(Color color) {
    SilentSet(prevColor = color);
}
#pragma endregion

#pragma region WriteUtils

void Console::Write(const std::string& text, Color color) {
    SetEval(color);
    std::cout << text;    
    SilentSet(prevColor);
}

void Console::WriteLine(const std::string& text, Console::Color color) {
    Console::Write(text, color);
    std::cout << "\n";
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
    SetConsoleCursorPosition(_cHan, sC);
}

std::string Console::ReadLine() {
    std::getline(std::cin, output);
    return output;
}
#pragma endregion