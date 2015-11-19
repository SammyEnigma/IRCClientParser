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

#ifndef WEATHER_H
#define WEATHER_H

#include "command.h"

namespace irc_parser
{

class Weather : public Command
{
public:
    Weather() = default;

    virtual void command(
        IRCCommandMessage const& irc_cmd_msg,
        IRCBot::Ptr const& irc_bot,
        std::string const& channel_to_reply) override;

    virtual std::vector<std::string> match() const override;
};

} // namespace irc_parser

#endif /* WEATHER_H */
