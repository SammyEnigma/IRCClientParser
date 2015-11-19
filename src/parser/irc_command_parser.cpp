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

#include "irc_command_parser.h"

#include <stdexcept>

namespace irc_parser
{

/*
Simple abnf

irc_command_message := *( SPACE ) "!" command *( 1*( SPACE ) params )
                    /= *( SPACE ) irc_bot_name [','] *( 1*( SPACE) params )
command := 1*(nocrlfspace)
params  := 1*(nocrlfspace)
nocrlfspace := %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
                ; any octet except NUL, CR, LF, and " "

*/

IRCCommandParser::IRCCommandParser(std::string const& command) :
    token_stream(command)
{
}

IRCCommandMessage IRCCommandParser::parse(std::string const& bot_name)
{
    IRCCommandMessage irc_cmd_msg;

    // *( SPACE )
    token_stream.while_has({Type::SPACE});
    if (!token_stream.has_next())
        throw std::runtime_error("Empty command");

    auto t = token_stream.look();

    if (t.type != Type::BANG)
        throw std::runtime_error("Expected '!' or bot name, " + bot_name + " got: " + token_type_name(t.type) + " " + t.token);
    token_stream.next();
/*
    // *( SPACE ) irc_bot_name [','] *( 1*( SPACE) params )
    if (t.type != Type::BANG)
    //{
        //auto check_bot_name = token_stream.while_has_not({Type::NUL, Type::CR, Type::LF,
                                                          //Type::SPACE, Type::SPECIAL, Type::COMMA});
        //t = token_stream.look();

        //if (check_bot_name != bot_name)
            throw std::runtime_error("Expected '!' or bot name, " + bot_name + " got: " + token_type_name(t.type) + " " + t.token);
    //}
    //else
    //{
        // "!"
        token_stream.next();
    //}
    */

    // [,] or *( SPACE )
    //token_stream.while_has({Type::SPACE, Type::COMMA});

    // command := 1*(nocrlfspace)
    irc_cmd_msg.command = token_stream.while_has_not({Type::NUL, Type::CR, Type::LF, Type::SPACE});
    if (token_stream.has_next())
        t = token_stream.look();

    // *( SPACE params ) := 1*(nocrlfspace)
    while (token_stream.has_next() && t.type == Type::SPACE)
    {
        token_stream.next();
        if (token_stream.has_next())
        {
            auto param = token_stream.while_has_not({Type::NUL, Type::CR, Type::LF, Type::SPACE});
            if (!param.empty())
                irc_cmd_msg.params.push_back(param);
            if (token_stream.has_next())
                t = token_stream.look();
        }
    }

    return irc_cmd_msg;
}

} // namespace irc_parser
