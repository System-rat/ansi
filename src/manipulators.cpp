// MIT License

// Copyright (c) 2022 System.rat

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "manipulators.h"
#include "ansi.h"
#include "internals/utils.h"
#include <iostream>

#define ANSI_IMPL(name, __CODE)                                                \
    std::ostream &name(std::ostream &os) {                                     \
        return write_if_term(os, "\e[" #__CODE, sizeof("\e[" #__CODE) - 1);    \
    }

namespace ansi {
namespace manip {

ANSI_IMPL(bold, 1m)          // NOLINT
ANSI_IMPL(faint, 2m)         // NOLINT
ANSI_IMPL(italic, 3m)        // NOLINT
ANSI_IMPL(underline, 4m)     // NOLINT
ANSI_IMPL(blink, 5m)         // NOLINT
ANSI_IMPL(reverse, 6m)       // NOLINT
ANSI_IMPL(hidden, 7m)        // NOLINT
ANSI_IMPL(strikethrough, 8m) // NOLINT

ANSI_IMPL(nobold, 21m)          // NOLINT
ANSI_IMPL(nofaint, 22m)         // NOLINT
ANSI_IMPL(noitalic, 23m)        // NOLINT
ANSI_IMPL(nounderline, 24m)     // NOLINT
ANSI_IMPL(noblink, 25m)         // NOLINT
ANSI_IMPL(noreverse, 26m)       // NOLINT
ANSI_IMPL(nohidden, 27m)        // NOLINT
ANSI_IMPL(nostrikethrough, 28m) // NOLINT

ANSI_IMPL(reset, 0m) // NOLINT

ANSI_IMPL(erase_screen_to_end, 0J)       // NOLINT
ANSI_IMPL(erase_screen_to_beginning, 1J) // NOLINT
ANSI_IMPL(erase_screen, 2J\r)            // NOLINT
ANSI_IMPL(erase_line_to_end, 0K)         // NOLINT
ANSI_IMPL(erase_line_to_beginning, 1K)   // NOLINT
ANSI_IMPL(erase_line, 2K\r)              // NOLINT

ANSI_IMPL(hide_cursor, ?25l) //NOLINT
ANSI_IMPL(show_cursor, ?25h) //NOLINT

auto modifier(TextModifier mod, bool reset) -> ManipulatorFunc {
    std::string escape;
    escape.reserve(5);
    escape += "\e[";
    // Edge case for the bold modifier because ANSI go brr
    if (reset && mod == ansi::Bold) {
        escape += "22";
    } else {
        escape += std::to_string(reset ? ((int)mod) + 20 : (int)mod);
    }
    escape += 'm';
    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(),
                             (std::streamsize)escape.length());
    });
}

auto color(Color c, bool is_background) -> ManipulatorFunc {
    std::string escape;
    escape.reserve(5);
    escape += "\e[";
    escape += std::to_string(is_background ? ((int)c) + 10 : (int)c);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), 5);
    });
}

auto color(uint8_t c, bool is_background) -> ManipulatorFunc {
    std::string escape;
    // Pessimistic allocation
    escape.reserve(11);
    escape += "\e[";
    escape += std::to_string(is_background ? 48 : 38);
    escape += ";5;";
    escape += std::to_string(c);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(),
                             (std::streamsize)escape.length());
    });
}

auto color(uint8_t r, uint8_t g, uint8_t b, bool is_background)
    -> ManipulatorFunc {
    std::string escape;
    // Pessimistic allocation
    escape.reserve(17);
    escape += "\e[";
    escape += std::to_string(is_background ? 48 : 38);
    escape += ";2;";
    escape += std::to_string(r) + ';';
    escape += std::to_string(g) + ';';
    escape += std::to_string(b);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(),
                             (std::streamsize)escape.length());
    });
}

auto move_cursor(CursorDirection direction, uint8_t amount) -> ManipulatorFunc {
    std::string escape("\e[");
    escape += std::to_string(amount);
    escape += (char)direction;

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(),
                             (std::streamsize)escape.length());
    });
}

auto move_cursor(uint8_t x, uint8_t y) -> ManipulatorFunc {
    std::string escape("\e[");
    escape += std::to_string(x);
    escape += ';';
    escape += std::to_string(y);
    escape += 'H';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(),
                             (std::streamsize)escape.length());
    });
}

auto move_cursor_home(std::ostream &os) -> std::ostream & {
    return os.write("\e[H", 3);
}

} // namespace manip
} // namespace ansi

auto operator<<(std::ostream &os, const ansi::ManipulatorFunc &&mf)
    -> std::ostream & {
    return mf(os);
}

#undef ANSI_IMPL