
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
#include "manipulators.h"
#include <ostream>
#include <sstream>
#include <tuple>
#include <vector>

namespace ansi {
/**
 * @brief Style classes for text
 * @details Contains classes for applying a set of styles to some text.
 *
 * Some examples:
 * @code
 * #include <iostream>
 * #include <Ansi>
 *
 * int main() {
 *  std::cout << ansi::styling::Style(ansi::styling::StyleColor(ansi::Yellow,
 * {ansi::Bold, ansi::Underline}).apply("Text")) << std::endl;
 * }
 * @endcode
 */
namespace styling {

/**
 * @brief Text styling
 * @details Holds styling information to be applied to text
 */
class Style {
  public:
    /**
     * @brief Constructs a new Style using a color and a list of
     * modifiers
     *
     * @param c The color of the text
     * @param m A list of modifiers to apply to text
     */
    Style(StyleColor c, std::initializer_list<ansi::TextModifier> &&m);

    /**
     * @brief Applies the style to text
     *
     * @param text The text to apply the style to
     * @return The manipulator to be used with operator<<
     * @note Resets all styling after the text is written to the ostream
     */
    const ManipulatorFunc apply(const char *text) const;

    /**
     * @brief Default constructor
     */
    Style();

    /**
     * @brief Copy constructor
     *
     * @param other The Style object to copy from
     */
    Style(const Style &other);

    /**
     * @brief Move constructor
     *
     * @param other The Style object to move from
     */
    Style(Style &&other);

    /**
     * @brief Trivial copy assignment operator
     *
     * @param other The Style object to copy from
     * @return The assigned object
     */
    Style &operator=(const Style &other);

    /**
     * @brief Trivial move assignment operator
     *
     * @param other The Style object to move from
     * @return The assigned object
     */
    Style &operator=(Style &&other);

  private:
    /**
     * @brief Holds the text color and type of color (ANSI, Term256, RGB)
     */
    StyleColor color;

    /**
     * @brief Holds the various text modifiers that can be applied (Bold,
     * Underline, etc.)
     */
    std::vector<ansi::TextModifier> modifiers;

    std::stringstream format_text;

    void compute_format_text();
};

} // namespace styling
} // namespace ansi
