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

#include "python.h"
#include "popen_command.h"

#include <sstream>

namespace irc_parser
{

namespace
{
std::string const python_match{"python"};
std::string const python_prefix{"python -c \"exec(\\\"import math\\\\nprint "};
std::string const python_postfix{"\\\")\""};
}

void Python::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{

    std::string python_cmd(python_prefix);
    for (auto const& p : irc_cmd_msg.params)
        python_cmd += p + " ";
    python_cmd.pop_back();
    python_cmd += python_postfix;

    auto python_ret = exec_popen_command(python_cmd);
    auto python = std::get<1>(python_ret);
    std::stringstream ss(python);
    std::vector<std::string> split_lines;
    std::string split_line;
    while (std::getline(ss, split_line, '\n'))
    if (!split_line.empty())
        split_lines.push_back(split_line);

    if (!split_lines.empty())
        for (auto const& line : split_lines)
            irc_bot->send_message(channel_to_reply, line);
}

std::vector<std::string> Python::match() const
{
    return {python_match};
}

}
