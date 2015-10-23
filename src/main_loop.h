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

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "irc_bot.h"
#include "irc_socket_reader.h"
#include "irc_parser.h"

#include <memory>
#include <list>

namespace irc_parser
{
class IRCBotMessageHandler;

class MainLoop
{
public:
    MainLoop();

    void add_irc_bot(IRCBot::Ptr& bot);

    void start();
    bool running() const;
    void read_sockets();

    void override_message_handler(
        std::function<std::shared_ptr<IRCBotMessageHandler>(void)> const& message_handler_builder);

private:
    void parse_irc_messages(std::string const& valid_messages, IRCBot::Ptr const& bot) const;

    IRCSocketReader irc_socket_reader;
    std::list<IRCBot::Ptr> bots;
    std::shared_ptr<IRCBotMessageHandler> irc_message_handler;
    bool active;
};

} // namespace irc_parser

#endif // MAIN_LOOP_H
