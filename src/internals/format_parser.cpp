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

#include "format_parser.h"
#include <cstring>
#include <string>
#include <vector>

// LEXER

Token Lexer::get_token() {
    if (is_at_end())
        return Token{.type = LexToken::EOFToken, .start = cursor};
    if (!is_in_context) {
        switch (*cursor) {
        case '#': {
            if (*(cursor + 1) == '\0')
                throw std::runtime_error("Unexpected end of input");

            if (*(cursor + 1) == '$') {
                auto tok =
                    Token{.type = LexToken::Var, .start = cursor, .length = 2};
                cursor += 2;

                return tok;
            } else if (*(cursor + 1) == '[') {
                is_in_context = true;
                return lex_format();
            } else {
                return lex_text();
            }
            break;
        }

        default:
            return lex_text();
            break;
        }
    } else {
        while (isspace(*cursor)) {
            cursor++;
        }

        return lex_format();
    }
}

Lexer::Lexer(const char *str) : input(str), cursor(str){};

bool Lexer::is_at_end(int extraOffset) {
    if (*(cursor + extraOffset) == '\0')
        return true;

    return false;
}

Token Lexer::lex_number() {
    offset = 0;
    while (isdigit(*(cursor + offset))) {
        offset++;
    }

    auto token =
        Token{.type = LexToken::Number, .start = cursor, .length = offset};
    cursor += offset;
    offset = 0;

    return token;
}

Token Lexer::lex_ident() {
    offset = 0;
    while (isalpha(*(cursor + offset))) {
        offset++;
    }

    auto token =
        Token{.type = LexToken::Ident, .start = cursor, .length = offset};
    cursor += offset;
    offset = 0;

    return token;
}

Token Lexer::lex_format() {
    offset = 0;

    if (is_at_end()) {
        throw std::runtime_error("unexpected end of input");
    }

    switch (*(cursor + offset)) {
    case '#': {
        if (*(cursor + offset + 1) == '[') {
            auto token =
                Token{.type = LexToken::Start, .start = cursor, .length = 2};
            cursor += 2;

            return token;
        } else {
            throw std::runtime_error(std::string("unexpected symbol: ") +
                                     std::string(cursor));
        }
        break;
    }
    case ',': {
        auto token =
            Token{.type = LexToken::Comma, .start = cursor, .length = 1};
        cursor++;

        return token;
        break;
    }
    case ']': {
        auto token = Token{.type = LexToken::End, .start = cursor, .length = 1};
        cursor++;
        is_in_context = false;

        return token;
        break;
    }

    default: {
        if (isalpha(*cursor)) {
            return lex_ident();
        } else if (isdigit(*cursor)) {
            return lex_number();
        } else if (isspace(*cursor)) {
            while (isspace(*cursor)) {
                cursor++;
            }

            return lex_format();
        } else {
            throw std::runtime_error(std::string("unexpected symbol: ") +
                                     std::string(cursor));
        }
        break;
    }
    }
}

Token Lexer::lex_text() {
    offset = 0;
    while (!is_at_end(offset)) {
        if (*(cursor + offset) == '#') {
            if (is_escape()) {
                offset += 2;
                continue;
            }

            switch (*(cursor + offset + 1)) {
            case '$':
            case '[':
                // I SPITE THEE
                // MY HERESY IS ABSOLUTE
                // BEHOLD THE CURSED ONE
                // THE DREADED UNDEFINED ONE
                // THE WORLD ENDER
                goto HERESY;
            }
        }
        offset++;
    }

HERESY:
    auto token =
        Token{.type = LexToken::Text, .start = cursor, .length = offset};
    cursor += offset;
    offset = 0;

    return token;
}

bool Lexer::is_escape() {
    if (is_at_end(1))
        return false;

    if (*(cursor + offset) != '#')
        return false;

    if (*(cursor + offset + 1) != '#')
        return false;

    return true;
}

// PARSER
