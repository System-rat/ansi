
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
#include <cstring>
#include <ostream>
#include <tuple>

namespace ansi {
namespace styling {

Style::Style(StyleColor c, std::initializer_list<ansi::TextModifier> m)
    : color(c), modifiers(m){};

const ansi::ManipulatorFunc Style::apply(const char *text) const {
    return ManipulatorFunc([=](std::ostream &os) -> std::ostream & {
        ManipulatorFunc mc = ansi::manip::color(ansi::Default);

        switch (color.type) {
        case StyleColor::ANSI:
            mc = ansi::manip::color(color.value.ansiColor);
            break;
        case StyleColor::Term256:
            mc = ansi::manip::color(color.value.term);
            break;
        case StyleColor::RGB:
            mc = ansi::manip::color(std::get<0>(color.value.rgb),
                                    std::get<1>(color.value.rgb),
                                    std::get<2>(color.value.rgb));
            break;

        default:
            break;
        }

        mc(os);

        for (auto &&m : modifiers) {
            auto mm = ansi::manip::modifier(m);

            mm(os);
        }

        os.write(text, strlen(text));

        ansi::manip::reset(os);

        return os;
    });
}

} // namespace styling
} // namespace ansi
