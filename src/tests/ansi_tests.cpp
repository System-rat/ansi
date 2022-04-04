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

#include <Ansi>
#include <gtest/gtest.h>
#include <sstream>

TEST(Ansi, StreamWriting) {
    std::stringstream s;
    s << ansi::manip::bold;
    s << ansi::manip::reset;

    ASSERT_EQ(s.str(), "\e[1m\e[0m");
}

TEST(Ansi, ColorWriting) {
    std::stringstream s;
    s << ansi::manip::color(ansi::Red);
    s << ansi::manip::color(ansi::Green, true);

    ASSERT_EQ(s.str(), "\e[31m\e[42m");
}

TEST(Ansi, Movement) {
    std::stringstream s;
    s << ansi::manip::move_cursor_home;
    s << ansi::manip::move_cursor(ansi::Up, 20);

    ASSERT_EQ(s.str(), "\e[H\e[20A");
}
