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

#ifndef IRC_PARSER_H
#define IRC_PARSER_H

#include "token_stream.h"

#include <string>
#include <vector>
#include <queue>

namespace irc_parser
{

struct Prefix
{
    std::string server;
    std::string nick;
    std::string user;
    std::string host;
};

struct IRCMessage
{
    Prefix prefix;
    std::string command;
    std::vector<std::string> params;
};

typedef std::queue<IRCMessage> IRCMessageQueue;

class IRCParser
{
public:
    explicit IRCParser(std::string const& text);

    IRCMessageQueue parse();

private:
    std::string parse_shortname();
    std::string parse_nick();
    std::string parse_user();
    std::string parse_host();
    Prefix parse_prefix();

    std::string parse_command();

    std::string parse_trailing();
    std::vector<std::string> parse_params();

    TokenStream token_stream;
   
};

} // namespace irc_parser

#endif // IRC_PARSER_H
