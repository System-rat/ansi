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
#include "ansi.h"
#include <functional>
#include <iostream>
#include <string>

#ifndef NODOC
#define __ANSI_MANIPULATOR(name) std::ostream &name(std::ostream &);
#endif

namespace ansi {

/**
 * @brief Represents a cursor movement direction
 * @details Represents the movement the cursor will make when moved and their
 * respective ANSI codes for movement
 */
enum CursorDirection {
    Up = 'A',
    Down = 'B',
    Right = 'C',
    Left = 'D',
    NextLine = 'E',
    PreviousLine = 'F',
    Column = 'G',
};

/**
 * @brief Manipulators for `std::ostream`
 * @details Contains various manipulators for ANSI Compliant terminals. Each one
 * applies a certain modifier to all following text (ie. bold, underline), or
 * moves the cursor around and erases characters. These are meant to be used
 * with the operator<< to change the output stream of stdout
 *
 * Some examples:
 * @code
 * #include <iostream>
 * #include <Ansi>
 *
 * int main() {
 *  std::cout << ansi::manip::bold << ansi::manip::color(ansi::Red) <<
 * std::endl;
 * }
 * @endcode
 */
namespace manip {

// Text effects

__ANSI_MANIPULATOR(bold)
__ANSI_MANIPULATOR(faint)
__ANSI_MANIPULATOR(italic)
__ANSI_MANIPULATOR(underline)
__ANSI_MANIPULATOR(blink)
__ANSI_MANIPULATOR(reverse)
__ANSI_MANIPULATOR(hidden)
__ANSI_MANIPULATOR(strikethrough)

// Global reset

__ANSI_MANIPULATOR(reset)

// Single effect resets

__ANSI_MANIPULATOR(nobold)
__ANSI_MANIPULATOR(nofaint)
__ANSI_MANIPULATOR(noitalic)
__ANSI_MANIPULATOR(nounderline)
__ANSI_MANIPULATOR(noblink)
__ANSI_MANIPULATOR(noreverse)
__ANSI_MANIPULATOR(nohidden)
__ANSI_MANIPULATOR(nostrikethrough)
__ANSI_MANIPULATOR(noreset)

// Erasing commands

__ANSI_MANIPULATOR(erase_screen_to_end)
__ANSI_MANIPULATOR(erase_screen_to_beginning)
__ANSI_MANIPULATOR(erase_screen)
__ANSI_MANIPULATOR(erase_line_to_end)
__ANSI_MANIPULATOR(erase_line_to_beginning)
__ANSI_MANIPULATOR(erase_line)

// Cursor visibility

__ANSI_MANIPULATOR(hide_cursor)
__ANSI_MANIPULATOR(show_cursor)

/**
 * @brief Creates a manipulator for the desired text styling
 *
 * @param mod Text styling
 * @param reset Should the styling be reset
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc modifier(const TextModifier mod, bool reset = false);

/**
 * @brief Creates a manipulator for the desired color
 *
 * @param c The color in question
 * @param is_background Is the color for the background color of the terminal or
 * the foreground. Defaults to foreground
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc color(const Color c, bool is_background = false);

/**
 * @brief Creates a manipulator for the desired color
 *
 * @param color The 256 term color to use
 * @param is_background Is the color for the background color of the terminal or
 * the foreground. Defaults to foreground
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc color(const uint8_t color, bool is_background = false);

/**
 * @brief Creates a manipulator for the desired color
 *
 * @param r Red channel
 * @param g Green channel
 * @param b Blue channel
 * @param is_background Is the color for the background color of the terminal or
 * the foreground. Defaults to foreground
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc color(const uint8_t r, const uint8_t g, const uint8_t b,
                            bool is_background = false);

/**
 * @brief Creates a manipulator to move the cursor
 *
 * @param direction The direction of the desired movement
 * @param amount The amount to move in that direction
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc move_cursor(const CursorDirection direction,
                                  const uint amount);

/**
 * @brief Creates a manipulator to move the cursor to the desired coordinates
 *
 * @param x X coord of the movement
 * @param y Y coord of the movement
 * @return The manipulator to be used with operator<<
 */
const ManipulatorFunc move_cursor(const uint x, const uint y);

/**
 * @brief Moves the cursor to home (0, 0)
 *
 * @param os The stream to manipulate
 * @return The manipulated stream
 */
std::ostream &move_cursor_home(std::ostream &os);

} // namespace manip
} // namespace ansi

/**
 * @brief The overloaded operator<< for applying a ManipulatorFunc on an
 * std::ostream
 *
 * @param os The stream to apply the ManipulatorFunc on
 * @param mf The manipulator
 * @return The manipulated stream
 */
std::ostream &operator<<(std::ostream &os, const ansi::ManipulatorFunc mf);

// Clean up utilities
#undef __ANSI_MANIPULATOR