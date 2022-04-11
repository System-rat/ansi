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
#include "manipulators.h"
#include <cstring>
#include <regex>
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
    case ':': {
        auto token =
            Token{.type = LexToken::Colon, .start = cursor, .length = 1};
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

Parser::Parser(const char *str) {
    has_modifiers = false;
    Lexer l = Lexer(str);
    std::vector<Token> tokens;
    Token token;

    while ((token = l.get_token()).type != LexToken::EOFToken) {
        tokens.push_back(token);
    }

    // Get the final EOF token
    tokens.push_back(token);
    this->tokens = tokens;
    cursor = this->tokens.data();
}

std::tuple<ansi::TextModifier, bool> string_to_modifier(std::string str) {
    if (str == "Bold")
        return {ansi::Bold, true};
    if (str == "Underline")
        return {ansi::Underline, true};
    if (str == "Italic")
        return {ansi::Italic, true};
    if (str == "Faint")
        return {ansi::Faint, true};
    if (str == "Blink")
        return {ansi::Blink, true};
    if (str == "Reverse")
        return {ansi::Reverse, true};
    if (str == "Hidden")
        return {ansi::Hidden, true};
    if (str == "Strikethrough")
        return {ansi::Strikethrough, true};

    return {ansi::Bold, false};
}

std::tuple<ansi::Color, bool> string_to_color(std::string str) {
    if (str == "Black")
        return {ansi::Black, true};
    if (str == "Red")
        return {ansi::Red, true};
    if (str == "Green")
        return {ansi::Green, true};
    if (str == "Yellow")
        return {ansi::Yellow, true};
    if (str == "Blue")
        return {ansi::Blue, true};
    if (str == "Magenta")
        return {ansi::Magenta, true};
    if (str == "Cyan")
        return {ansi::Cyan, true};
    if (str == "White")
        return {ansi::White, true};
    if (str == "Default")
        return {ansi::Default, true};

    return {ansi::Default, false};
}

std::string Parser::get_format_string() {
    while (cursor->type != LexToken::EOFToken) {
        switch (cursor->type) {
        case LexToken::Text:
            builder.write(cursor->start, cursor->length);
            advance();
            break;

        case LexToken::Start:
            format();
            advance();
            break;
        case LexToken::Var:
            builder.write("%s", 2);
            advance();
            break;
        default:
            throw std::runtime_error("Invalid format string");
        }
    }

    builder << ansi::manip::reset;

    std::string built_string = builder.str();
    // Clean the escaped #
    if (built_string.find("##") != -1)
        return std::regex_replace(built_string, std::regex("##"), "#");

    return built_string;
}

uint8_t get_num(std::string str) {
    int num = std::stoi(str);

    if (num > std::numeric_limits<uint8_t>::max() ||
        num < std::numeric_limits<uint8_t>::min()) {
        throw std::runtime_error("Number is too large for color format: " +
                                 str);
    }

    return num;
}

void Parser::format() {
    if (peek()->type == LexToken::EOFToken)
        throw std::runtime_error("Unexpected end of string");

    has_modifiers = false;
    builder << ansi::manip::reset;
    advance();

    while (cursor->type != LexToken::End) {
        if (has_modifiers)
            consume(LexToken::Comma);

        if (cursor->type == LexToken::Ident) {
            auto mod =
                string_to_modifier(std::string(cursor->start, cursor->length));

            if (std::get<1>(mod)) {
                builder << ansi::manip::modifier(std::get<0>(mod));
                has_modifiers = true;
                advance();
                continue;
            }

            auto col =
                string_to_color(std::string(cursor->start, cursor->length));

            if (std::get<1>(col)) {
                builder << ansi::manip::color(std::get<0>(col));
                has_modifiers = true;
                advance();
                continue;
            }

            throw std::runtime_error(
                "Unexpected identifier: " +
                std::string(cursor->start, cursor->length));
        }

        if (cursor->type == LexToken::Number) {
            uint8_t num = get_num(std::string(cursor->start, cursor->length));
            advance();

            if (cursor->type != LexToken::Colon) {
                builder << ansi::manip::color(num);
                has_modifiers = true;
                continue;
            }

            consume(LexToken::Colon);

            if (cursor->type != LexToken::Number)
                throw std::runtime_error("Expected number");
            uint8_t num2 = get_num(std::string(cursor->start, cursor->length));

            advance();
            consume(LexToken::Colon);

            if (cursor->type != LexToken::Number)
                throw std::runtime_error("Expected number");
            uint8_t num3 = get_num(std::string(cursor->start, cursor->length));

            advance();

            builder << ansi::manip::color(num, num2, num3);
            has_modifiers = true;
            continue;
        }

        if (cursor->type == LexToken::EOFToken) {
            throw std::runtime_error("Unexpected end of string");
        }

        throw std::runtime_error("Unexpected token: " +
                                 std::string(cursor->start, cursor->length));
    }
}

std::string token_to_string(LexToken token) {
    switch (token) {
    case LexToken::Start:
        return "#[";
    case LexToken::End:
        return "]";
    case LexToken::Comma:
        return ",";
    case LexToken::Ident:
        return "Identifier";
    case LexToken::Var:
        return "#$";
    case LexToken::Text:
        return "Text";
    case LexToken::Number:
        return "Number";
    case LexToken::Colon:
        return ":";
    case LexToken::EOFToken:
        return "EOF";
    }
}

void Parser::consume(LexToken type) {
    if (cursor->type != type)
        throw std::runtime_error("Expected symbol: " + token_to_string(type));

    advance();
}

Token *Parser::peek() {
    if (cursor->type != LexToken::EOFToken)
        return cursor + 1;

    return cursor;
}

void Parser::advance() {
    if (cursor->type != LexToken::EOFToken)
        cursor++;
}
