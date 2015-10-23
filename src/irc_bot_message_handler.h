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

#ifndef IRC_BOT_MESSAGE_HANDLER_H
#define IRC_BOT_MESSAGE_HANDLER_H

#include "irc_parser.h"

#include <memory>

namespace irc_parser
{
class IRCBot;
class PrivMessageData;

class IRCBotMessageHandler
{
public:
    IRCBotMessageHandler();

    void handle(IRCMessage const& irc_msg, std::shared_ptr<IRCBot> const& irc_bot);

private:
    std::shared_ptr<PrivMessageData> priv_message_data;
};

} // namespace irc_parser

#endif // IRC_BOT_MESSAGE_HANDLER_H
