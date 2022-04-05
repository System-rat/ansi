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

std::vector<Token> Lexer::generate_tokens() {
    while (!is_at_end()) {
        switch (*cursor) {
        case '#': {
            if (*(cursor + 1) == '\0')
                throw std::runtime_error("Unexpected end of input");

            if (*(cursor + 1) == '$') {
                tokens.push_back(Token{.type = LexToken::Var});

                cursor += 2;
            } else if (*(cursor + 1) == '[') {
                lex_format();
            } else {
                lex_text();
            }
            break;
        }

        default:
            lex_text();
            break;
        }
    }

    return tokens;
}

Lexer::Lexer(const char *str) : input(str), cursor(str){};

bool Lexer::is_at_end(int extraOffset) {
    if (*(cursor + extraOffset) == '\0')
        return true;

    return false;
}

void Lexer::lex_number() {
    offset = 0;
    while (isdigit(*(cursor + offset))) {
        offset++;
    }

    tokens.push_back(
        Token{.type = LexToken::Number, .value = std::string(cursor, offset)});
    cursor += offset;
    offset = 0;
}

void Lexer::lex_ident() {
    offset = 0;
    while (isalpha(*(cursor + offset))) {
        offset++;
    }

    tokens.push_back(
        Token{.type = LexToken::Ident, .value = std::string(cursor, offset)});
    cursor += offset;
    offset = 0;
}

void Lexer::lex_format() {
    offset = 0;
    tokens.push_back(Token{.type = LexToken::Start});
    cursor += 2;
    while (*cursor != ']') {
        char c = *cursor;

        if (isspace(c)) {
            cursor++;
            continue;
        }

        if (isdigit(c)) {
            lex_number();
            continue;
        }

        if (isalpha(c)) {
            lex_ident();
            continue;
        }

        if (c == ',') {
            tokens.push_back(Token{.type = LexToken::Comma});
            cursor++;
            continue;
        }

        if (c == '\0') {
            throw std::runtime_error("Unexpected end of input");
        }

        throw std::runtime_error("The hell is this character?");
    }

    tokens.push_back(Token{.type = LexToken::End});
    cursor++;
}

void Lexer::lex_text() {
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
                goto END;
            }
        }
        offset++;
    }

END:
    tokens.push_back(
        Token{.type = LexToken::Text, .value = std::string(cursor, offset)});
    cursor += offset;
    offset = 0;
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
