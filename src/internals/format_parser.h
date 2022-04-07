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

// Careful, ye who tread down these cursed lands of newby parser
// implementations, for you might find unoptimized horrors beyond comprehension

#pragma once
#include "ansi.h"
#include <sstream>
#include <string>
#include <vector>

enum struct LexToken { Start, End, Comma, Ident, Var, Text, Number, EOFToken };

struct Token {
    LexToken type;

    const char *start;
    int length;

    std::string to_string() const { return std::string(start, length); }
};

class Lexer {
  public:
    Lexer(const char *inputText);

    Token get_token();

  private:
    bool is_in_context = false;

    const char *input;

    const char *cursor;

    int offset;

    bool is_at_end(int extraOffset = 0);

    Token lex_text();

    Token lex_format();

    Token lex_ident();

    Token lex_number();

    void advance();

    bool is_escape();
};
