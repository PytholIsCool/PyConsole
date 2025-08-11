#pragma once

#if defined(_WIN32)
#include <Windows.h>
#else

// Minimal COLORREF replacement for non-Windows builds
using COLORREF = unsigned int;
#ifndef RGB
inline constexpr COLORREF RGB(unsigned r, unsigned g, unsigned b) {
    return (COLORREF)((r) | ((g) << 8) | ((b) << 16));
}

inline constexpr unsigned GetRValue(COLORREF c) { return (c) & 0xFFu; }
inline constexpr unsigned GetGValue(COLORREF c) { return ((c) >> 8) & 0xFFu; }
inline constexpr unsigned GetBValue(COLORREF c) { return ((c) >> 16) & 0xFFu; }
#endif

#endif