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
    const char *txt = "#[Bold, 123]Text #$";

    std::vector<Token> expectedOutput = {
        Token{.type = LexToken::Start, .start = "#[", .length = 2},
        Token{.type = LexToken::Ident, .start = "Bold", .length = 4},
        Token{.type = LexToken::Comma, .start = ",", .length = 1},
        Token{.type = LexToken::Number, .start = "123", .length = 3},
        Token{.type = LexToken::End, .start = "]", .length = 1},
        Token{.type = LexToken::Text, .start = "Text ", .length = 5},
        Token{.type = LexToken::Var, .start = "#$", .length = 2}};

    Lexer l = Lexer(txt);

    Token t;

    std::vector<Token> generated;

    while ((t = l.get_token()).type != LexToken::EOFToken) {
        generated.push_back(t);
    }

    for (int i = 0; i < generated.size(); i++) {
        ASSERT_EQ(generated[i].to_string(), expectedOutput[i].to_string());
        ASSERT_EQ(generated[i].type, expectedOutput[i].type);
    }
}

TEST(Internals, LexerError) {
    Lexer l("#[##]");
    l.get_token();
    ASSERT_ANY_THROW(l.get_token());

    Lexer valid("#[Bold, Green]Text and stuff #[1234, DoesNotExist]More stuff "
                "#[]Reset");

    ASSERT_NO_THROW({
        for (;;) {
            if (valid.get_token().type == LexToken::EOFToken)
                break;
        }
    });
}