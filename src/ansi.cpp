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

#include "ansi.h"
#ifndef _WIN32
#include "termios.h"
#include "unistd.h"
#include <array>
#else
#include <windows.h>
#endif

namespace ansi {

#ifndef _WIN32

// Yeah, yeah, globals, who gives a fuck
std::array<termios, 3> original;

auto set_raw_tty(int fd) -> void {
    if (!(bool)isatty(fd)) {
        return;
    }

    termios t;
    tcgetattr(fd, &t);

    original[fd] = t;

    t.c_iflag = t.c_iflag & ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    t.c_oflag = t.c_oflag & ~OPOST;
    t.c_cflag = t.c_cflag & ~(CSIZE | PARENB);
    t.c_cflag = t.c_cflag | CS8;
    t.c_lflag = t.c_lflag & ~(ECHO | ICANON | IEXTEN | ISIG);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSAFLUSH, &t);
}

auto reset_tty(int fd) -> void {
    if (!(bool)isatty(fd)) {
        return;
    }

    tcsetattr(fd, TCSAFLUSH, &original[fd]);
}

#else

auto enable_ansi_windows() -> int {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return GetLastError();
    }

    return 0;
}

#endif

} // namespace ansi
