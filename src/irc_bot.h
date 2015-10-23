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

#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "socket.h"

namespace irc_parser
{

enum class ModeOp : char
{
    MINUS = '-',
    PLUS  = '+',
    NONE  = ' ',
};

enum class Mode : char
{
    STATUS = 'O',
    OP     = 'o',
    VOICE  = 'v',
    ANONYMOUS  = 'v',
    INVITE     = 'i',
    MODERATE   = 'm',
    NO_MESSAGE = 'n',
    QUIET = 'q',
    PRIVATE = 'p',
    SECRET  = 's',
    SERVER_REOP = 'r',
    TOPIC_MOD = 't',
    PASSWORD  = 'k',
    LIMIT = 'l',
    BAN = 'b',
    EXCEPTION = 'e',
    INVITE_OVERRIDE = 'I',
    NONE = ' ',
};

typedef std::vector<std::string> Servers;
typedef std::vector<std::string> Channels;
typedef std::vector<std::string> Keys;
typedef std::vector<Mode> Modes;
typedef std::vector<std::string> ModeParams;

struct ModeMessage
{
    ModeOp op;
    Modes modes;
    ModeParams mode_params;
};

class IRCBot
{
public:
    typedef std::shared_ptr<IRCBot> Ptr;

    explicit IRCBot(IRCServerInfo const& server_info);

    void join_channel(Channels const& channels, Keys const& keys = {}) const;
    void part_channel(Channels const& channels, std::string const& part_message = {}) const;
    void mode_channel(std::string const& channel, std::vector<ModeMessage> const& mode_msg = {}) const;

    void away(std::string const& text = {});

    void pong(Servers const& servers);

    void send_message(std::string const& to, std::string const& msg) const;

    void init_bot(std::function<void()> const& callback = {});
    void connected();

    bool active() const;

    std::string name() const;
    int irc_socket() const;

private:
    void send_to_server(std::string const& msg) const;

    std::function<void()> connected_callback;
    Socket socket;
    std::string bot_name;
};

} // namespace irc_parser

#endif // IRC_BOT_H
