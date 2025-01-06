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

    static void SetForegroundColor(Color color);
    static void Write(const std::string& text, Color color = Color::Placeholder);
    static void WriteLine(const std::string& text, Color color = Color::Placeholder);
    static void Debug(const std::string& text);
    static void Error(const std::string& text);
    static void Clear();
    static std::string ReadLine();
};
#endif