#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <windows.h>
#include <iostream>
#include <list>
#include <string>

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
    static void ReplaceFirst(std::string& str, const std::string& token, int value) {
        str.replace(str.find(token), token.length(), std::to_string(value));
    }

    static void ReplaceFirst(std::string& str, const std::string& token, float value) {
        str.replace(str.find(token), token.length(), std::to_string(value));
    }

    static void ReplaceFirst(std::string& str, const std::string& token, double value) {
        str.replace(str.find(token), token.length(), std::to_string(value));
    }

    static void ReplaceFirst(std::string& str, const std::string& token, bool value) {
        str.replace(str.find(token), token.length(), value ? "true" : "false");
    }

    static void ReplaceFirst(std::string& str, const std::string& token, const std::string& value) {
        str.replace(str.find(token), token.length(), value);
    }

    static void ReplaceFirst(std::string& str, const std::string& token, const char* value) {
        str.replace(str.find(token), token.length(), value ? value : "(null)");
    }

    template<typename T>
    static auto ReplaceFirst(std::string& str, const std::string& token, const T& value)
        -> decltype(std::declval<std::ostringstream&>() << value, void()) {
        std::ostringstream oss;
        oss << value;
        str.replace(str.find(token), token.length(), oss.str());
    }

    static void ReplaceFirst(std::string& str, const std::string& token, ...) {
        str.replace(str.find(token), token.length(), "[[unprintable]]");
    }

    template<typename T, typename... Rest>
    static void ReplaceFirstAt(std::string& str, int index, const T& value, const Rest&... rest) {
        ReplaceFirst(str, "{" + std::to_string(index) + "}", value);
        if constexpr (sizeof...(rest) > 0) {
            ReplaceFirstAt(str, index + 1, rest...);
        }
    }

    template<typename... Args>
    static void FormatReplace(std::string& str, const Args&... args) {
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
            if (isDebugMode == false)
                return;
            std::string formatted = formatStr;
            FormatReplace(formatted, std::forward<Args>(args)...);
            Debug(formatted);
        }
};
#endif
