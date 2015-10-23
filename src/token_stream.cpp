//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* * Copyright (C) 2015 Brandon Schaefer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Authored by: Brandon Schaefer <brandontschaefer@gmail.com>
*/

#include <algorithm>
#include <functional>

#include "token_stream.h"

namespace irc_parser
{

namespace
{
bool is_letter(char const c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char const c)
{
    return c >= '0' && c <= '9';
}

bool is_special(char const c)
{
    return (c >= '\x5b' && c <= '\x60') || (c >= '\x7b' && c <= '\x7d');
}

std::string pull_string(Lexer& l, std::function<bool(char const)> const& filter)
{
    std::string filter_str;

    char c = l.look();
    do {
        l.next();
        filter_str += c;
        c = l.look();
    } while (filter(c));

    return filter_str;
}
}

TokenStream::TokenStream(std::string const& text) :
    lexer(text)
{
    // Tokenize the lexer!
    while (lexer.has_next())
    {
        char const c = lexer.look();
        if (is_letter(c))
        {
            std::string letters = pull_string(lexer, is_letter);
            tokens.push_back({Type::LETTER, letters});
        }
        else if (is_digit(c))
        {
            std::string digits = pull_string(lexer, is_digit);
            tokens.push_back({Type::DIGIT, digits});
        }
        else if (c == ':')
        {
            tokens.push_back({Type::COLON, std::string(1, c)});
            lexer.next();
        }
        else if (c == '@')
        {
            tokens.push_back({Type::AT, std::string(1, c)});
            lexer.next();
        }
        else if (c == '-')
        {
            tokens.push_back({Type::DASH, std::string(1, c)});
            lexer.next();
        }
        else if (c == '.')
        {
            tokens.push_back({Type::DOT, std::string(1, c)});
            lexer.next();
        }
        else if (c == '!')
        {
            tokens.push_back({Type::BANG, std::string(1, c)});
            lexer.next();
        }
        //else if (c == ',')
        //{
            //tokens.push_back({Type::COMMA, std::string(1, c)});
            //lexer.next();
        //}
        else if (c == '/')
        {
            tokens.push_back({Type::FSLASH, std::string(1, c)});
            lexer.next();
        }
        else if (c == ' ')
        {
            tokens.push_back({Type::SPACE, std::string(1, c)});
            lexer.next();
        }
        else if (c == '\0')
        {
            tokens.push_back({Type::NUL, std::string(1, c)});
            lexer.next();
        }
        else if (c == '\r')
        {
            tokens.push_back({Type::CR, std::string(1, c)});
            lexer.next();
        }
        else if (c == '\n')
        {
            tokens.push_back({Type::LF, std::string(1, c)});
            lexer.next();
        }
        else if (is_special(c))
        {
            tokens.push_back({Type::SPECIAL, std::string(1, c)});
            lexer.next();
        }
        else // ignore anything we dont expect
        {
            // Need to save it for the message side of the commands
            tokens.push_back({Type::NONE, std::string(1, c)});
            lexer.next();
        }
    }
}

std::string TokenStream::while_has(std::initializer_list<Type> const& token_types)
{
    std::string built_string;
    auto t = tokens[current];

    while(std::find(std::begin(token_types), std::end(token_types), t.type) != std::end(token_types) &&
          has_next())
    {
        built_string += t.token;
        next();
        if (has_next())
            t = look();
    }
    
    return built_string;
}

std::string TokenStream::while_has_not(std::initializer_list<Type> const& token_types)
{
    std::string built_string;
    auto t = tokens[current];

    while(std::find(std::begin(token_types), std::end(token_types), t.type) == std::end(token_types) &&
          has_next())
    {
        built_string += t.token;
        next();
        if (has_next())
            t = look();
    }

    return built_string;
}

Token TokenStream::look(int k) const
{
    return tokens[current + k];
}

Token TokenStream::get_next()
{
    return tokens[++current];
}

bool TokenStream::has_next() const
{
    return current < tokens.size();
}

void TokenStream::next()
{
    current++;
}

std::string token_type_name(Type const& t)
{
    switch (t)
    {
        case Type::LETTER:
            return "Letter";
        case Type::DIGIT:
            return "Digit";
        case Type::COLON:
            return "Colon";
        case Type::AT:
            return "At";
        case Type::DASH:
            return "Dash";
        case Type::DOT:
            return "Dot";
        case Type::FSLASH:
            return "Forward Slash";
        case Type::SPACE:
            return "Space";
        case Type::BANG:
            return "Bang";
        //case Type::COMMA:
            //return "Comma";
        case Type::SPECIAL:
            return "Special";
        case Type::NUL:
            return "Null";
        case Type::CR:
            return "Cariage Return";
        case Type::LF:
            return "New line";
        case Type::NONE:
        default:
            return "Unhandled character";
    }
}

} // namespace irc_parser
