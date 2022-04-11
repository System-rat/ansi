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

#pragma once

/**
 * @brief Convenience macro for `ansi::format_str`
 */
#define ANSI(str) ansi::format_str(str)

namespace ansi {

/**
 * @brief Returns a cached ANSI format string
 * @details Parses the input string using a tiny format language and returns a
 * cached `printf` compatible string.
 *
 * The formatting is as follows:
 *  - All normal text is treated as text
 *  - `#[` begins a format modifier
 *  - Inside the format modifier color and text modifiers can be specified
 * (Bold, Red, 220, 20:10:2)
 *  - Color can be specified as either an ANSI color, a Term256 color or rgb
 *  - RGB is 3 numbers separated by colons, ie. 220:150:30
 *  - `]` closes the format modifier
 *  - `#$' gets replaced with a `%s`
 *  - `##` escapes the `#` character
 *
 * An example:
 * @code
 * #include <iostream>
 * #include <Ansi>
 *
 * int main() {
 *     std::cout << ansi::format_str(
 *         "#[Bold, Underline, Red]Underlined and ##bolded## text"
 *     ) << std::endl;
 * }
 *
 * @endcode
 *
 * @param fstr ANSI format string
 * @return A cached printf compatible format string
 *
 * @throws std::runtime_exception The format string is invalid and cannot be
 * parsed
 */
const char *format_str(const char *fstr);

} // namespace ansi
