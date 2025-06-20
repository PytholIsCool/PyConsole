#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <regex>
#include <iomanip>
#include <type_traits>

class Console {
public:
    static bool isDebugMode;

    // Output
	static void Write(const std::string& text);
    static void Write(const std::string& text, COLORREF color);
	static void WriteLine(const std::string& text);
    static void WriteLine(const std::string& text, COLORREF color);

    // Input
    static std::string ReadLine();
    static char Read();         // getchar-style
    static char ReadKey();      // immediate char

    // Color
    static void SetForegroundColor(COLORREF color);
    static void SetBackgroundColor(COLORREF color);
    static void ResetColor();

    // Cursor / Screen
    static void SetCursorPosition(int x, int y);
    static void Clear();

    // Debug helpers
    static void Debug(const std::string& text);  // styled debug output
    static void Error(const std::string& text);  // styled error output

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
    // RGB
    static COLORREF foregroundColor;
    static COLORREF backgroundColor;

	static bool isSetUp;
    static void ConsoleSetup();
    static void WriteRGBText(COLORREF color, const std::string& text);

    // Templates And Formatting
    static void FixAnonymousPlaceholders(std::string& str);

    static void ReplaceFirst(std::string& str, const std::string& token, int value);

    static void ReplaceFirst(std::string& str, const std::string& token, float value);

    static void ReplaceFirst(std::string& str, const std::string& token, double value);

    static void ReplaceFirst(std::string& str, const std::string& token, bool value);

    static void ReplaceFirst(std::string& str, const std::string& token, const std::string& value);

    static void ReplaceFirst(std::string& str, const std::string& token, const char* value);

    static void ReplaceFirst(std::string& str, const std::string& token, ...);

    static void ReplaceFormatted(std::string& str, const std::string& token, const std::string& fmt, uint64_t value);

    template<typename T>
    static auto ReplaceFirst(std::string& str, const std::string& token, const T& value)
        -> decltype(std::declval<std::ostringstream&>() << value, void()) {
        std::ostringstream oss;
        oss << value;

        size_t pos = str.find(token);
        if (pos != std::string::npos && pos + token.length() <= str.length()) {
            str.replace(pos, token.length(), oss.str());
            return;
        }

        // Add this log to track why nothing happened
        Console::Error(">>> Nothing replaced for token '{}'", token);
    }

    template<typename T>
    static void ReplaceToken(std::string& str, int index, const T& value) {
        std::ostringstream finalOutput;
        std::regex pattern(R"(\{)" + std::to_string(index) + R"((?::([^}]+))?\})");

        std::sregex_iterator it(str.begin(), str.end(), pattern);
        std::sregex_iterator end;

        size_t lastEnd = 0;
        for (; it != end; ++it) {
            finalOutput << str.substr(lastEnd, it->position() - lastEnd);

            std::ostringstream oss;
            std::string fmt = (*it)[1];
            char fillChar = ' ';
            int width = 0;
            bool upper = false;
            bool hex = false;

            if (!fmt.empty()) {
                if (fmt.find('X') != std::string::npos) { hex = true; upper = true; }
                else if (fmt.find('x') != std::string::npos) hex = true;

                size_t digitStart = fmt.find_first_of("0123456789");
                if (digitStart != std::string::npos) {
                    width = std::stoi(fmt.substr(digitStart));
                    if (fmt[digitStart] == '0') fillChar = '0';
                }
            }

            if constexpr (std::is_same_v<T, bool>) {
                oss << std::boolalpha << value;
            }
            else if constexpr (std::is_integral_v<T>) {
                if (hex) {
                    oss << std::setfill(fillChar) << std::setw(width)
                        << (upper ? std::uppercase : std::nouppercase)
                        << std::hex << value;
                }
                else {
                    oss << std::setfill(fillChar) << std::setw(width) << value;
                }
            }
            else {
                oss << value;
            }

            finalOutput << oss.str();
            lastEnd = it->position() + it->length();
        }

        finalOutput << std::string(str.begin() + lastEnd, str.end());
        str = finalOutput.str();

        // Also replace raw {N}
        std::string simpleToken = "{" + std::to_string(index) + "}";
        std::ostringstream raw;
        if constexpr (std::is_same_v<T, bool>)
            raw << std::boolalpha << value;
        else
            raw << value;

        size_t pos = 0;
        while ((pos = str.find(simpleToken, pos)) != std::string::npos) {
            str.replace(pos, simpleToken.length(), raw.str());
            pos += raw.str().length();
        }
    }

    template<typename T, typename... Rest>
    static void ReplaceFirstAt(std::string& str, int index, const T& value, const Rest&... rest) {
        ReplaceToken(str, index, value);
        if constexpr (sizeof...(rest) > 0)
            ReplaceFirstAt(str, index + 1, rest...);
    }

    template<typename... Args>
    static void FormatReplace(std::string& str, const Args&... args) {
        FixAnonymousPlaceholders(str);
        ReplaceFirstAt(str, 0, args...);
    }

public:
    template<typename... Args>
    static void WriteLine(const std::string& formatStr, Args&&... args) {
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        WriteLine(formatted);
    }

    template<typename... Args>
    static void WriteLine(COLORREF color, const std::string& formatStr, Args&&... args) {
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        WriteLine(formatted, color);
    }

    template<typename... Args>
    static void Write(const std::string& formatStr, Args&&... args) {
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        Write(formatted);
    }

    template<typename... Args>
    static void Write(COLORREF color, const std::string& formatStr, Args&&... args) {
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        Write(formatted, color);
    }

    template<typename... Args>
    static void Error(const std::string& formatStr, Args&&... args) {
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        Error(formatted);
    }

    template<typename... Args>
    static void Debug(const std::string& formatStr, Args&&... args) {
        if (!isDebugMode) return;
        std::string formatted = formatStr;
        FormatReplace(formatted, std::forward<Args>(args)...);
        Debug(formatted);
    }
};
#endif
