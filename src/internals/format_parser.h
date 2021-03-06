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

// Careful, ye who tread down these cursed lands of newbie parser
// implementations, for you might find unoptimized horrors beyond comprehension

#pragma once
#include "ansi.h"
#include <sstream>
#include <string>
#include <vector>

enum struct LexToken {
    Start,
    End,
    Comma,
    Ident,
    Var,
    Text,
    Number,
    Colon,
    EOFToken
};

struct Token {
    LexToken type;

    const char *start;
    int length;

    auto to_string() const -> std::string { return std::string(start, length); }
};

class Lexer {
  public:
    explicit Lexer(const char *text);

    auto get_token() -> Token;

  private:
    bool is_in_context = false;

    const char *input;

    const char *cursor;

    int offset;

    auto is_at_end(int extraOffset = 0) -> bool;

    auto lex_text() -> Token;

    auto lex_format() -> Token;

    auto lex_ident() -> Token;

    auto lex_number() -> Token;

    auto advance() -> void;

    auto is_escape() -> bool;
};

class Parser {
  public:
    explicit Parser(const char *text);

    auto get_format_string() -> std::string;

  private:
    std::vector<Token> tokens;

    Token *cursor;

    std::stringstream builder;

    bool has_modifiers;

    auto advance() -> void;

    auto peek() -> Token *;

    auto consume(LexToken type) -> void;

    auto format() -> void;
};
