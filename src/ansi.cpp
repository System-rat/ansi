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
// Once again, suck it windows
#include "termios.h"

namespace ansi {

// Yeah, yeah, globals, who gives a fuck
termios original[2];

void set_raw_tty(int fd) {
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

void reset_tty(int fd) { tcsetattr(fd, TCSAFLUSH, &original[fd]); }
} // namespace ansi

#endif