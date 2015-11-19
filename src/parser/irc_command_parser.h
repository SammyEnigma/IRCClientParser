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

#ifndef IRC_COMMAND_PARSER_H
#define IRC_COMMAND_PARSER_H

#include "token_stream.h"

#include <string>
#include <vector>

namespace irc_parser
{

struct IRCCommandMessage
{
    std::string user;
    std::string command;
    std::vector<std::string> params;
};

class IRCCommandParser
{
public:
    explicit IRCCommandParser(std::string const& command);

    IRCCommandMessage parse(std::string const& bot_name);

private:
    std::string parse_command();
    std::vector<std::string> parse_params();

    TokenStream token_stream;
};

} // namespace irc_parser

#endif // IRC_COMMAND_PARSER_H
