#pragma once
#include <sstream>
#include <string>
#include "Console.h"

namespace PyConsole::Fmt {
    namespace detail {
        inline void replace_first(std::string& s, const std::string& token, const std::string& val) {
            const auto pos = s.find(token);
            if (pos != std::string::npos) s.replace(pos, token.size(), val);
        }
        template <typename T>
        inline std::string to_string_any(const T& v) {
            std::ostringstream oss;
            oss << v;
            return oss.str();
        }
        inline std::string to_string_any(const std::string& v) { return v; }
        inline std::string to_string_any(const char* v) { return v ? std::string(v) : std::string("(null)"); }
        inline std::string to_string_any(bool v) { return v ? "true" : "false"; }
    }

    template <typename... Args>
    inline std::string Format(std::string fmt, const Args&... args) {
        int index = 0;
        (detail::replace_first(fmt, "{" + std::to_string(index++) + "}", detail::to_string_any(args)), ...);
        return fmt;
    }

    template <typename... Args>
    inline void Write(const std::string& fmt, const Args&... args) {
        Console::Write(Format(fmt, args...));
    }
    template <typename... Args>
    inline void Write(COLORREF c, const std::string& fmt, const Args&... args) {
        Console::Write(Format(fmt, args...), c);
    }
    template <typename... Args>
    inline void WriteLine(const std::string& fmt, const Args&... args) {
        Console::WriteLine(Format(fmt, args...));
    }
    template <typename... Args>
    inline void WriteLine(COLORREF c, const std::string& fmt, const Args&... args) {
        Console::WriteLine(Format(fmt, args...), c);
    }
    template <typename... Args>
    inline void Debug(const std::string& fmt, const Args&... args) {
        if (!Console::isDebugMode) return;
        Console::Debug(Format(fmt, args...));
    }
    template <typename... Args>
    inline void Error(const std::string& fmt, const Args&... args) {
        Console::Error(Format(fmt, args...));
    }
}