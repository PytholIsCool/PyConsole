#include <iostream>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <conio.h>
#include "Console.hpp"
#include <sstream>
#include <iomanip>

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
    std::string msg = "[ERROR] ";
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

#pragma region Template Utils

void Console::FixAnonymousPlaceholders(std::string& str) {
    std::regex anonymousRegex(R"(\{\})");
    int counter = 0;
    std::smatch match;
    std::string result;
    std::string::const_iterator searchStart(str.cbegin());

    while (std::regex_search(searchStart, str.cend(), match, anonymousRegex)) {
        result.append(searchStart, match.prefix().second);
        result += "{" + std::to_string(counter++) + "}";
        searchStart = match.suffix().first;
    }
    result.append(searchStart, str.cend());
    str = std::move(result);
}

void Console::ReplaceFirst(std::string& str, const std::string& token, int value) {
    std::smatch match;
    std::regex formatRegex(R"(\{(\d+)(?::([^}]+))?\})");

    std::string::const_iterator searchStart(str.cbegin());
    while (std::regex_search(searchStart, str.cend(), match, formatRegex)) {
        // Match token index
        std::string matchIndex = match[1].str();
        std::string tokenIndex = token.substr(1, token.length() - 2); // "{0}" -> "0"

        if (matchIndex == tokenIndex) {
            // Safe to replace this one
            std::ostringstream oss;
            std::string fmt = match[2];
            char fillChar = ' ';
            int width = 0;
            bool upper = false;
            bool hex = false;

            if (!fmt.empty()) {
                if (fmt.find('X') != std::string::npos) {
                    hex = true;
                    upper = true;
                }
                else if (fmt.find('x') != std::string::npos) {
                    hex = true;
                }

                size_t digitStart = fmt.find_first_of("0123456789");
                if (digitStart != std::string::npos) {
                    width = std::stoi(fmt.substr(digitStart));
                    if (fmt[digitStart] == '0') fillChar = '0';
                }
            }

            if (hex) {
                oss << std::setfill(fillChar) << std::setw(width)
                    << (upper ? std::uppercase : std::nouppercase)
                    << std::hex << value;
            }
            else {
                oss << std::setfill(fillChar) << std::setw(width) << value;
            }

            // Use match.str() to get the actual string being replaced
            std::string matchStr = match.str();
            size_t pos = str.find(matchStr);
            size_t len = matchStr.length();

            Console::Debug("Replacing '{}' at pos {}, len {}, with '{}'", matchStr, pos, len, oss.str());

            if (pos == std::string::npos || pos + len > str.length()) {
                Console::Error("!!! INVALID REPLACE: pos={}, len={}, str.len={}", pos, len, str.length());
                return;
            }

            str.replace(pos, len, oss.str());
            return;
        }

        searchStart = match.suffix().first;
    }

    Console::Debug("Marker3");
    // Fallback
    size_t pos = str.find(token);
    if (pos != std::string::npos)
        str.replace(pos, token.length(), std::to_string(value));
}

void Console::ReplaceFirst(std::string& str, const std::string& token, float value) {
    str.replace(str.find(token), token.length(), std::to_string(value));
}

void Console::ReplaceFirst(std::string& str, const std::string& token, double value) {
    str.replace(str.find(token), token.length(), std::to_string(value));
}

void Console::ReplaceFirst(std::string& str, const std::string& token, bool value) {
    str.replace(str.find(token), token.length(), value ? "true" : "false");
}

void Console::ReplaceFirst(std::string& str, const std::string& token, const std::string& value) {
    str.replace(str.find(token), token.length(), value);
}

void Console::ReplaceFirst(std::string& str, const std::string& token, const char* value) {
    str.replace(str.find(token), token.length(), value ? value : "(null)");
}

void Console::ReplaceFirst(std::string& str, const std::string& token, ...) {
    str.replace(str.find(token), token.length(), "[[unprintable]]");
}

void Console::ReplaceFormatted(std::string& str, const std::string& token, const std::string& fmt, uint64_t value) {
    std::ostringstream oss;
    char fillChar = ' ';
    int width = 0;
    bool upper = false;
    bool hex = false;

    if (fmt.find('X') != std::string::npos) {
        hex = true;
        upper = true;
    }
    else if (fmt.find('x') != std::string::npos) {
        hex = true;
    }

    auto padStart = fmt.find(':');
    if (padStart != std::string::npos) {
        std::string padSpec = fmt.substr(padStart + 1);
        size_t numStart = padSpec.find_first_of("0123456789");
        if (numStart != std::string::npos) {
            width = std::stoi(padSpec.substr(numStart));
        }
        if (padSpec.find('0') != std::string::npos) {
            fillChar = '0';
        }
    }

    if (hex) {
        oss << std::hex << (upper ? std::uppercase : std::nouppercase);
    }
    oss << std::setfill(fillChar) << std::setw(width) << value;

    str.replace(str.find(token), token.length(), oss.str());
}

#pragma endregion
