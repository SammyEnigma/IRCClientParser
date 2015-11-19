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

#ifndef POKER_21_TABLE_H
#define POKER_21_TABLE_H

#include "command.h"
#include "poker_21.h"

#include <map>

namespace irc_parser
{

class Poker21Policy : public Command
{
public:
    Poker21Policy() = default;

    virtual void command(
        IRCCommandMessage const& irc_cmd_msg,
        IRCBot::Ptr const& irc_bot,
        std::string const& channel_to_reply) override;

    virtual std::vector<std::string> match() const override;

private:
    std::map<std::string, Poker21> game_per_channel;
};

} // namespace irc_parser

#endif // POKER_21_TABLE_H
