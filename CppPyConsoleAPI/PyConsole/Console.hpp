#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <list>

class Console {
public:
    enum Color : int {
        Black = 0,
        Blue = 1,
        DarkGreen = 2,
        SkyBlue = 3,
        Red = 4,
        DarkPurple = 5,
        Orange = 6,
        LightGray = 7,
        Gray = 8,
        LightBlue = 9,
        Green = 10,
        BabyBlue = 11,
        Rose = 12,
        Purple = 13,
        LightYellow = 14,
        White = 15,
        Placeholder = -0
    };

    // Output
    static void Write(const std::string& text, Color color = Placeholder);
    static void WriteLine(const std::string& text, Color color = Placeholder);
    static void WriteLine(Color color = Placeholder); // newline with color

    // Input
    static std::string ReadLine();
    static char Read();         // getchar-style
    static char ReadKey();      // immediate char

    // Color
    static void SetForegroundColor(Color color);
    static void SetBackgroundColor(Color color);
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
};
#endif