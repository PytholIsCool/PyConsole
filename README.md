# PyConsole

PyConsole is a lightweight C++ console API that extends standard input/output with color support, formatting, cursor control, and other utilities. It is designed to be simple to use and easy to integrate.

## Features

- Simple API for reading and writing text
- RGB color output using ANSI escape codes 
- Debug and error helpers with styled output
- Cursor control (move, get position, clear screen)
- Window control (set title, get dimensions)
- Input helpers (check for key, read key without Enter)
- Pause utility for prompts

## Planned
- Support for Linux and MacOS
- More diverse log levels
- Multiple log sources
- Making the entire API thread safe
- Easy integration with alloc-consoles or other types of consoles where appropriate/needed handles aren't provided by default
- Seamless formatted output with `{0}`, `{1}`, `{2}` style placeholders (It is supported but you have to use PyConsole::Fmt rather than PyConsole::Console)

## Getting Started
```cpp
#include <PyConsole/Console.h>

using namespace PyConsole;

int main() {
    Console::SetTitle("PyConsole Demo");
    Console::SetForegroundColor(RGB(0, 255, 0));
    Console::WriteLine("Hello, World in green!");

    Console::Pause();
    return 0;
}
```

## Questions
### Can I use this in my project?
> Go for it. This is intended for anybody to use, whether it be an individual, a team or as unlikely as it is, a corporation.
