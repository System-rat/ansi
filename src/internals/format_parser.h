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
#include <vector>

enum struct LexToken { Start, End, Comma, Ident, Var, Text, Number };

struct Token {
    LexToken type;

    std::string value;
};

namespace ast {
namespace tokens {

enum struct TopLevel { Modifier, Text, Variable };

enum struct Modifier { Color, Style };

} // namespace tokens

struct Modifier {
    tokens::Modifier type;
    union {
        ansi::StyleColor color;
        ansi::TextModifier modifier;
    } value;
};

struct TopLevel {
    tokens::TopLevel type;
    union {
        Modifier modifier;
        std::string text;
        // Variable is just a type level identifier
    } value;
};

struct AST {
    std::vector<TopLevel> topLevel;
};

} // namespace ast

class Lexer {
  public:
    Lexer(const char *inputText);

    std::vector<Token> generate_tokens();

  private:
    const char *input;

    const char *cursor;

    int offset;

    std::vector<Token> tokens;

    bool is_at_end(int extraOffset = 0);

    void lex_text();

    void lex_format();

    void lex_ident();

    void lex_number();

    void advance();

    bool is_escape();
};

class Parser {
  public:
    Parser(const std::vector<LexToken> inputTokens);

    ast::AST generate_ast();

  private:
    const std::vector<LexToken> tokens;

    ast::AST ast;

    LexToken *cursor;

    int offset;

    std::stringstream fstr;

    bool is_at_end(int extraOffset = 0);

    void advance();
};
