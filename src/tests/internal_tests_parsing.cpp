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

#include "../internals/format_parser.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(Internals, Lexing) {
    std::vector<Token> expectedOutput = {
        Token{.type = LexToken::Start},
        Token{.type = LexToken::Ident, .value = "Bold"},
        Token{.type = LexToken::Comma},
        Token{.type = LexToken::Number, .value = "123"},
        Token{.type = LexToken::End},
        Token{.type = LexToken::Text, .value = "Text "},
        Token{.type = LexToken::Var}};

    std::vector<Token> generated =
        Lexer("#[Bold, 123]Text #$").generate_tokens();

    for (int i = 0; i < generated.size(); i++) {
        ASSERT_EQ(generated[i].value, expectedOutput[i].value);
        ASSERT_EQ(generated[i].type, expectedOutput[i].type);
    }
}