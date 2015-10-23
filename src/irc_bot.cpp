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

#include "irc_bot.h"

namespace irc_parser
{

IRCBot::IRCBot(IRCServerInfo const& server_info) :
    socket(server_info),
    bot_name(server_info.nick)
{
}

void IRCBot::send_message(std::string const& to, std::string const& msg) const
{
    send_to_server({"PRIVMSG " + to + " :" + msg + "\r\n"});
}

// Parameters: <nickname>
void IRCBot::nick(std::string const& nick) const
{
    send_to_server({"NICK " + nick + "\r\n"});
}

// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
void IRCBot::join_channel(Channels const& channels, Keys const& keys) const
{
    std::string join_command{"JOIN "};

    for (auto const& c : channels)
        join_command += c + ",";
    join_command.pop_back(); // remove extra ,
    join_command += " ";

    if (!keys.empty())
    {
        for (auto const& k : keys)
            join_command += k + ",";
        join_command.pop_back(); // remove extra ,
    }

    join_command += "\r\n";

    send_to_server(join_command);
}

// Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
void IRCBot::part_channel(Channels const& channels, std::string const& part_message) const
{
    std::string part_command{"PART "};

    for (auto const& c : channels)
        part_command += c + ",";
    part_command.pop_back(); // remove extra ,
    part_command += " " + part_message + "\r\n";

    send_to_server(part_command);
}

// Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
void IRCBot::mode_channel(std::string const& channel, std::vector<ModeMessage> const& mode_msg) const
{
    std::string mode_command{"MODE " + channel + " "};// + static_cast<char>(op)};
    for (auto const& msg : mode_msg)
    {
        mode_command += static_cast<char>(msg.op);
        for (auto const& mode : msg.modes)
            mode_command += static_cast<char>(mode);

        mode_command += " ";
        for (auto const& mode_params : msg.mode_params)
        {
            mode_command += mode_params;
            mode_command += " ";
        }
    }

    mode_command += "\r\n";

    send_to_server(mode_command);
}

// Parameters: [ <text> ]
void IRCBot::away(std::string const& text)
{
    send_to_server("AWAY " + text);
}

// Parameters: <server> [ <server2> ]
void IRCBot::pong(Servers const& servers)
{
    std::string pong_message{"PONG :"};
    for (auto const& s : servers)
        pong_message += s + " ";
    pong_message.pop_back();

    pong_message += "\r\n";

    send_to_server(pong_message);
}

void IRCBot::send_to_server(std::string const& msg) const
{
    socket.send_to_socket(msg);
}

std::string IRCBot::name() const
{
    return bot_name;
}

bool IRCBot::active() const
{
    return socket.created();
}

int IRCBot::irc_socket() const
{
    return socket();
}

void IRCBot::init_bot(std::function<void()> const& callback)
{
    socket.create_and_connect();
    connected_callback = callback;
}

void IRCBot::connected()
{
    if (connected_callback)
        connected_callback();
}

} // namespace irc_parser
