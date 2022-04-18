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
#define ANSI_MANIPULATOR(name) std::ostream &name(std::ostream &);
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

#ifndef NODOC

ANSI_MANIPULATOR(bold)          // NOLINT
ANSI_MANIPULATOR(faint)         // NOLINT
ANSI_MANIPULATOR(italic)        // NOLINT
ANSI_MANIPULATOR(underline)     // NOLINT
ANSI_MANIPULATOR(blink)         // NOLINT
ANSI_MANIPULATOR(reverse)       // NOLINT
ANSI_MANIPULATOR(hidden)        // NOLINT
ANSI_MANIPULATOR(strikethrough) // NOLINT

// Global reset

ANSI_MANIPULATOR(reset) // NOLINT

// Single effect resets

ANSI_MANIPULATOR(nobold)          // NOLINT
ANSI_MANIPULATOR(nofaint)         // NOLINT
ANSI_MANIPULATOR(noitalic)        // NOLINT
ANSI_MANIPULATOR(nounderline)     // NOLINT
ANSI_MANIPULATOR(noblink)         // NOLINT
ANSI_MANIPULATOR(noreverse)       // NOLINT
ANSI_MANIPULATOR(nohidden)        // NOLINT
ANSI_MANIPULATOR(nostrikethrough) // NOLINT
ANSI_MANIPULATOR(noreset)         // NOLINT

// Erasing commands

ANSI_MANIPULATOR(erase_screen_to_end)       // NOLINT
ANSI_MANIPULATOR(erase_screen_to_beginning) // NOLINT
ANSI_MANIPULATOR(erase_screen)              // NOLINT
ANSI_MANIPULATOR(erase_line_to_end)         // NOLINT
ANSI_MANIPULATOR(erase_line_to_beginning)   // NOLINT
ANSI_MANIPULATOR(erase_line)                // NOLINT

// Cursor visibility

ANSI_MANIPULATOR(hide_cursor) // NOLINT
ANSI_MANIPULATOR(show_cursor) // NOLINT

#endif

/**
 * @brief Creates a manipulator for the desired text styling
 *
 * @param mod Text styling
 * @param reset Should the styling be reset
 * @return The manipulator to be used with operator<<
 */
auto modifier(TextModifier mod, bool reset = false) -> ManipulatorFunc;

/**
 * @brief Creates a manipulator for the desired color
 *
 * @param c The color in question
 * @param is_background Is the color for the background color of the terminal or
 * the foreground. Defaults to foreground
 * @return The manipulator to be used with operator<<
 */
auto color(Color c, bool is_background = false) -> ManipulatorFunc;

/**
 * @brief Creates a manipulator for the desired color
 *
 * @param color The 256 term color to use
 * @param is_background Is the color for the background color of the terminal or
 * the foreground. Defaults to foreground
 * @return The manipulator to be used with operator<<
 */
auto color(uint8_t color, bool is_background = false) -> ManipulatorFunc;

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
auto color(uint8_t r, uint8_t g, uint8_t b, bool is_background = false)
    -> ManipulatorFunc;

/**
 * @brief Creates a manipulator to move the cursor
 *
 * @param direction The direction of the desired movement
 * @param amount The amount to move in that direction
 * @return The manipulator to be used with operator<<
 */
auto move_cursor(CursorDirection direction, uint8_t amount) -> ManipulatorFunc;

/**
 * @brief Creates a manipulator to move the cursor to the desired coordinates
 *
 * @param x X coord of the movement
 * @param y Y coord of the movement
 * @return The manipulator to be used with operator<<
 */
auto move_cursor(uint8_t x, uint8_t y) -> ManipulatorFunc;

/**
 * @brief Moves the cursor to home (0, 0)
 *
 * @param os The stream to manipulate
 * @return The manipulated stream
 */
auto move_cursor_home(std::ostream &os) -> std::ostream &;

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
auto operator<<(std::ostream &os, const ansi::ManipulatorFunc &&mf)
    -> std::ostream &;

// Clean up utilities
#undef ANSI_MANIPULATOR