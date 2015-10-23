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

#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "lexer.h"

#include <vector>

namespace irc_parser
{

enum class Type
{
// letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
    LETTER,
// digit      =  %x30-39                 ; 0-9
    DIGIT,
// ':'
    COLON,
// '@'
    AT,
// '-'
    DASH,
// '.'
    DOT,
// '/'
    FSLASH,
// SPACE      =  %x20        ; space character
    SPACE,
// '!'
    BANG,
// ','
    //COMMA,
//   special    =  %x5B-60 / %x7B-7D
//                 ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
    SPECIAL,
// '\0'
    NUL,
// '\r'
    CR,
// '\n'
    LF,
    NONE
};

struct Token
{
    Type type{Type::NONE};
    std::string token;
};

// For errors, to return a name of a type
std::string token_type_name(Type const& t);

class TokenStream
{
public:
    explicit TokenStream(std::string const& text);

    Token look(int k = 0) const;
    Token get_next();

    std::string while_has(std::initializer_list<Type> const& token_types);
    std::string while_has_not(std::initializer_list<Type> const& token_types);

    bool has_next() const;
    void next();
private:
    Lexer lexer;
    std::vector<Token> tokens;
    size_t current{0};
};

} // namespace irc_parser

#endif // TOKEN_STREAM_H
