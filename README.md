# PyConsole

PyConsole is a lightweight C++ console API that extends standard input/output with color support, formatting, cursor control, and other utilities. It is designed to be simple to use and easy to integrate.

## Features

- Simple API for reading and writing text
- RGB color output using ANSI escape codes
- Formatted output with `{0}`, `{1}`, `{2}` style placeholders
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
