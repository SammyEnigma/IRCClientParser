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

#include "youtube.h"

namespace irc_parser
{

namespace
{
    std::string const youtube_match{"youtube"};
}

void Youtube::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    if (!irc_cmd_msg.params.empty())
    {
        std::string search;
        for (auto const& p : irc_cmd_msg.params)
            search += p + "+";
        search.pop_back();
        irc_bot->send_message(channel_to_reply, "https://www.youtube.com/results?q=" + search);
    }
    else
    {
        irc_bot->send_message(channel_to_reply, "[ERROR] youtube expects 1 or more arguments");
    }
}

std::vector<std::string> Youtube::match() const
{
    return {youtube_match};
}

}
