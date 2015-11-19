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

#include "poker_21_policy.h"

namespace irc_parser
{

namespace
{
    std::string const poker_match{"poker"};
}

void Poker21Policy::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    //auto poker = &game_per_channel[channel_to_reply];
    //if (poker->user_waiting() == irc_cmd_msg.user)
    {
        //poker->handle_user_commnad(irc_cmd_msg);
    }
    /*
    play
    stop

    hit
      player gets 1 card added to hand
    stay
      player keeps current hand
    double
      player doubles bet and takes 1 more card
    split
      cards have to be the same, split into two hands (same bet each)
      each hand gets 1 card added to it
      player plays out each hand
    surrender
      only on first turn player folds hand and gets half bet back
    */
}

std::vector<std::string> Poker21Policy::match() const
{
    return {poker_match};
}

} // namespace irc_parser
