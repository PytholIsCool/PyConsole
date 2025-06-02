#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "Console.hpp"

bool Console::isSetUp = false;
bool Console::isDebugMode = false;
HANDLE _cHan = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO _csbi;
COORD sC = { 0, 0 };
std::string output;

#pragma region ColorHandler

COLORREF Console::foregroundColor = RGB(255, 255, 255);
COLORREF Console::backgroundColor = RGB(0, 0, 0);

void Console::SetForegroundColor(COLORREF color) {
	foregroundColor = color;
}

void Console::SetBackgroundColor(COLORREF color) {
	backgroundColor = color;
}

void Console::ResetColor() {
	foregroundColor = RGB(255, 255, 255);
    backgroundColor = RGB(0, 0, 0);
}

#pragma endregion

#pragma region WriteUtils

void Console::Write(const std::string& text) {
    if (!isSetUp)
        ConsoleSetup();
	WriteRGBText(foregroundColor, text);
}

void Console::Write(const std::string& text, COLORREF color) {
	if (!isSetUp)
        ConsoleSetup();
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

void Console::Debug(const std::string& text) {
    if (!isDebugMode)
        return;
    std::string msg = "[DEBUG] ";
    WriteLine(msg.append(text), RGB(255, 255, 224));
    std::cout << "\n";
}

void Console::Error(const std::string& text) {
    std::string msg = "[DEBUG] ";
    WriteLine(msg.append(text), RGB(128, 0, 0));
    std::cout << "\n";
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
    WriteLine("Press ENTER to continue...", RGB(192, 192, 192));
    (void)ReadLine();
}
#pragma endregion

#pragma region Private Utils
void Console::ConsoleSetup() {
    // Enabling ANSI
    DWORD dwMode = 0;
    GetConsoleMode(_cHan, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(_cHan, dwMode);
    isSetUp = true;

	// Enabling UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void Console::WriteRGBText(COLORREF color, const std::string& text) {
    std::cout << char(27) << "[38;2;"   // i was using "\x1b" which is the literal string but it can get "corrupted" at runtime
		<< (int)GetRValue(color) << ";" // the alternative is using char(27) which directly forces the escape character to be inserted at runtime
		<< (int)GetGValue(color) << ";" // this allows for custom RGB colors using ANSI escape codes
		<< (int)GetBValue(color) << "m" // i also tried saving this CPP file as UTF-8 with BOM but it didn't work
        << text
        << char(27) << "[0m";
}

#pragma endregion