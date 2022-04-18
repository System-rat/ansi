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

#include "styling.h"
#include "ansi.h"
#include "internals/utils.h"
#include <cstring>
#include <ostream>
#include <tuple>

namespace ansi {
namespace styling {

Style::Style(StyleColor c, std::initializer_list<ansi::TextModifier> &&m)
    : color(std::move(c)), modifiers(m) {
    compute_format_text();
};

Style::Style() = default;

Style::Style(const Style &other)
    : color(other.color), modifiers(other.modifiers) {
    compute_format_text();
}

Style::Style(Style &&other) noexcept
    : color(std::move(other.color)), modifiers(std::move(other.modifiers)),
      format_text(std::move(other.format_text)){};

auto Style::operator=(const Style &other) -> Style & {
    this->~Style();
    new (this) Style(other);
    return *this;
}

auto Style::operator=(Style &&other) noexcept -> Style & {
    this->~Style();
    new (this) Style(std::move(other));
    return *this;
}

auto Style::apply(const char *text) const -> ManipulatorFunc {
    const std::string ftext = format_text.str();
    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        write_if_term(os, ftext.c_str(), (std::streamsize)ftext.length());

        os.write(text, (std::streamsize)strlen(text));

        ansi::manip::reset(os);

        return os;
    });
}

auto Style::compute_format_text() -> void {
    format_text.clear();
    format_text.str(std::string());
    ManipulatorFunc mc = ansi::manip::color(ansi::Default);

    switch (color.type) {
    case StyleColor::ANSI:
        mc = ansi::manip::color(color.ansiColor);
        break;
    case StyleColor::Term256:
        mc = ansi::manip::color(color.term);
        break;
    case StyleColor::RGB:
        mc = ansi::manip::color(std::get<0>(color.rgb), std::get<1>(color.rgb),
                                std::get<2>(color.rgb));
        break;

    default:
        break;
    }

    mc(format_text);

    for (auto &&m : modifiers) {
        auto mm = ansi::manip::modifier(m);

        mm(format_text);
    }
}

} // namespace styling
} // namespace ansi
