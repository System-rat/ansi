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
#include <functional>
#include <ostream>

/**
 * @brief The root namespace of the ANSI manipulators library
 * @details Contains various unility structures for ANSI related text outputing
 */
namespace ansi {

/**
 * @brief A wrapper for a manipulator lambda
 * @details A type-safe holder for an std::function manipulator lambda.
 * Supports the call operator for use with the global overload of operator<<
 * With std::ostream
 */
class ManipulatorFunc {
    typedef std::function<std::ostream &(std::ostream &)> fn;

    fn func;

  public:
    /**
     * @brief Construct a new ManipulatorFunc from a manipulator compatible
     * lambda
     *
     * @param function The manipulator lambda with the signiture of
     * std::ostream&(std::ostream&)
     */
    explicit ManipulatorFunc(fn &&function) { func = function; }

    /**
     * @brief Apply the internal lambda to the std::ostream
     *
     * @param os The stream to manipulate
     * @return The manipulated stream
     */
    inline std::ostream &operator()(std::ostream &os) const { return func(os); }
};

/**
 * @brief The ANSI Terminal Simple Color representation
 * @details Represents the basic ANSI codes for color that are used withing the
 * manipulators
 */
enum Color {
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37,
    /**
     * @brief Resets the color back to it's original terminal color
     */
    Default = 39,
};

/**
 * @brief ANSI text graphics modes
 * @details Represents the ANSI text graphics modes for changing the styling of
 * text
 */
enum TextModifier {
    Bold = 1,
    Faint = 2,
    Italic = 3,
    Underline = 4,
    Blink = 5,
    Reverse = 7,
    Hidden = 8,
    Strikethrough = 9
};

/**
 * @brief Holds color information compatible with ANSI standards
 * @details Holds various formats of colors for use with other functions/classes
 * to represent ANSI compatible color formats
 */
struct StyleColor {
    /**
     * @brief The format type of the color
     * @details Term256 is for chosing one of the 256 builtin colors, RGB is for
     * chosing the Red, Green, and Blue channels individually (on supported
     * terminals), and ANSI is for the 8 ANSI colors
     */
    enum ColorType { Term256, RGB, ANSI } type;

    /**
     * @brief Holds the color value depending on the color type
     * @details Holds the color number for Term256, the 3 rgb values, or one of
     * the 8 ANSI color values
     * @note This union is tagged with the type field of StyleColor
     */
    union {
        ansi::Color ansiColor;
        std::tuple<uint8_t, uint8_t, uint8_t> rgb;
        uint8_t term;
    };

    /**
     * @brief Constructs a new StyleColor using the provided ANSI color and sets
     * the color type of ANSI
     * @param c The ANSI color to use
     */
    StyleColor(Color c) : type(ANSI), ansiColor(c){};
    /**
     * @brief Constructs a new StyleColor using a color number and sets the
     * color type to Term256
     *
     * @param c The color number to use
     */
    StyleColor(uint8_t c) : type(Term256), term(c){};
    /**
     * @brief Constructs a new StyleColor using 3 values for the Red, Green, and
     * Blue color channel and sets the color type to RGB
     *
     * @param r The value for the Red channel
     * @param g The value for the Green channel
     * @param b The value for the Blue channel
     */
    StyleColor(uint8_t r, uint8_t g, uint8_t b)
        : type(RGB), rgb(std::tuple<uint8_t, uint8_t, uint8_t>(r, g, b)){};

    /**
     * @brief Default constructor
     */
    StyleColor() : type(ANSI), ansiColor(ansi::Default){};

    /**
     * @brief Copy constructor
     *
     * @param other The StyleColor object to copy from
     */
    StyleColor(const StyleColor &other) {
        // If only c++ actually supported Sum types this wouldn't need to be
        // done
        type = other.type;
        switch (type) {
        case ANSI:
            ansiColor = other.ansiColor;
            break;

        case Term256:
            term = other.term;
            break;
        case RGB:
            rgb = other.rgb;
        default:
            // Unreachable
            break;
        }
    }

    /**
     * @brief Move constructor
     *
     * @param other The StyleColor object to move from
     */
    StyleColor(StyleColor &&other) {
        type = other.type;
        switch (type) {
        case ANSI:
            ansiColor = other.ansiColor;
            break;

        case Term256:
            term = other.term;
            break;
        case RGB:
            rgb = std::move(other.rgb);
        default:
            // Unreachable
            break;
        }
    }

    /**
     * @brief Trivial copy assignment operator
     *
     * @param rhs The StyleColor object to copy from
     * @return The assigned object
     */
    StyleColor &operator=(const StyleColor &rhs) {
        this->~StyleColor();
        new (this) StyleColor(rhs);
        return *this;
    }

    /**
     * @brief Trivial move assignment operator
     *
     * @param rhs The StyleColor object to copy from
     * @return The assigned object
     */
    StyleColor &operator=(StyleColor &&rhs) {
        this->~StyleColor();
        new (this) StyleColor(std::move(rhs));
        return *this;
    }

    /**
     * @brief Discriminated union destructor
     */
    ~StyleColor() {
        switch (type) {
        case RGB:
            rgb.~tuple();
            break;

        default:
            // No need to do anything special with the other values
            break;
        }
    }
};

/**
 * @brief Sets the terminal to RAW output
 *
 * @param fd The file descriptor of the output to set
 * @warning This should only be called once, if called more than once or from a
 * separate thread it's undefined behavior
 */
void set_raw_tty(int fd);

/**
 * @brief Reset the terminal to original functionality
 *
 * @param fd The file descriptor of the output to set
 * @warning This should only be called once, if called more than once or from a
 * separate thread it's undefined behavior
 */
void reset_tty(int fd);

} // namespace ansi