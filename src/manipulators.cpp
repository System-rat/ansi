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

// TODO: Make this a header library?

#define __ANSI_IMPL(name, __CODE)                                              \
    std::ostream &name(std::ostream &os) {                                     \
        return write_if_term(os, "\e[" #__CODE, sizeof("\e[" #__CODE) - 1);    \
    }

namespace ansi {
namespace manip {

__ANSI_IMPL(bold, 1m)
__ANSI_IMPL(faint, 2m)
__ANSI_IMPL(italic, 3m)
__ANSI_IMPL(underline, 4m)
__ANSI_IMPL(blink, 5m)
__ANSI_IMPL(reverse, 6m)
__ANSI_IMPL(hidden, 7m)
__ANSI_IMPL(strikethrough, 8m)

__ANSI_IMPL(nobold, 21m)
__ANSI_IMPL(nofaint, 22m)
__ANSI_IMPL(noitalic, 23m)
__ANSI_IMPL(nounderline, 24m)
__ANSI_IMPL(noblink, 25m)
__ANSI_IMPL(noreverse, 26m)
__ANSI_IMPL(nohidden, 27m)
__ANSI_IMPL(nostrikethrough, 28m)

__ANSI_IMPL(reset, 0m)

__ANSI_IMPL(erase_screen_to_end, 0J)
__ANSI_IMPL(erase_screen_to_beginning, 1J)
__ANSI_IMPL(erase_screen, 2J\r)
__ANSI_IMPL(erase_line_to_end, 0K)
__ANSI_IMPL(erase_line_to_beginning, 1K)
__ANSI_IMPL(erase_line, 2K\r)

__ANSI_IMPL(hide_cursor, ?25l)
__ANSI_IMPL(show_cursor, ?25h)

// TODO: Too many copies maybe?

const ManipulatorFunc modifier(const TextModifier mod, bool reset) {
    std::string escape;
    escape.reserve(5);
    escape += "\e[";
    // Edge case for the bold modifier because ANSI go brrrr
    if (reset && mod == ansi::Bold) {
        escape += "22";
    } else {
        escape += std::to_string(reset ? ((int)mod) + 20 : (int)mod);
    }
    escape += 'm';
    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), escape.length());
    });
}

const ManipulatorFunc color(const Color c, const bool is_background) {
    std::string escape;
    escape.reserve(5);
    escape += "\e[";
    escape += std::to_string(is_background ? ((int)c) + 10 : (int)c);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), 5);
    });
}

const ManipulatorFunc color(const uint8_t c, const bool is_background) {
    std::string escape;
    // Pesimistic allocation
    escape.reserve(11);
    escape += "\e[";
    escape += std::to_string(is_background ? 48 : 38);
    escape += ";5;";
    escape += std::to_string(c);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), escape.length());
    });
}

const ManipulatorFunc color(const uint8_t r, const uint8_t g, const uint8_t b,
                            bool is_background) {
    std::string escape;
    // Pesimistic allocation
    escape.reserve(17);
    escape += "\e[";
    escape += std::to_string(is_background ? 48 : 38);
    escape += ";2;";
    escape += std::to_string(r) + ';';
    escape += std::to_string(g) + ';';
    escape += std::to_string(b);
    escape += 'm';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), escape.length());
    });
}

const ManipulatorFunc move_cursor(const CursorDirection direction,
                                  const uint8_t amount) {
    std::string escape("\e[");
    escape += std::to_string(amount);
    escape += direction;

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), escape.length());
    });
}

const ManipulatorFunc move_cursor(const uint8_t x, const uint8_t y) {
    std::string escape("\e[");
    escape += std::to_string(x);
    escape += ';';
    escape += std::to_string(y);
    escape += 'H';

    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        return write_if_term(os, escape.c_str(), escape.length());
    });
}

std::ostream &move_cursor_home(std::ostream &os) { return os.write("\e[H", 3); }

} // namespace manip
} // namespace ansi

std::ostream &operator<<(std::ostream &os, const ansi::ManipulatorFunc &&mf) {
    return mf(os);
}

#undef __ANSI_IMPL