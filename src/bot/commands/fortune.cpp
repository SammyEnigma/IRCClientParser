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

#include "fortune.h"
#include "popen_command.h"

#include <sstream>

namespace irc_parser
{

namespace
{
std::string const fortune_match{"fortune"};
}

void Fortune::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    auto fortune_ret = exec_popen_command("fortune");
    auto fortune = std::get<1>(fortune_ret);
    std::stringstream ss(fortune);
    std::vector<std::string> split_lines;
    std::string split_line;
    while (std::getline(ss, split_line, '\n'))
        if (!split_line.empty())
            split_lines.push_back(split_line);

    if (!split_lines.empty())
        for (auto const& s : split_lines)
            irc_bot->send_message(channel_to_reply, s);
}

std::vector<std::string> Fortune::match() const
{
    return {fortune_match};
}

}
