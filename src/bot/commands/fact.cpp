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

#include "fact.h"
#include "popen_command.h"

namespace irc_parser
{

namespace
{
std::string const fact_match{"fact"};
std::string const fact_command{
    "wget randomfunfacts.com -O - 2>/dev/null | grep \\<strong\\> | sed \"s;^.*<i>\\(.*\\)</i>.*$;\\1;\""};
}

void Fact::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    auto fact_ret = exec_popen_command(fact_command);
    auto fact = std::get<1>(fact_ret);
    if (!fact.empty())
        irc_bot->send_message(channel_to_reply, fact);
}

std::vector<std::string> Fact::match() const
{
    return {fact_match};
}

}
