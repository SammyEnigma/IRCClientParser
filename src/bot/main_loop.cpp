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

#include "irc_bot_message_handler.h"
#include "main_loop.h"
#include "parser/irc_parser.h"

#include <algorithm>
#include <iostream>

namespace irc_parser
{

#define REPORT_ERROR(msg) msg + (" @:" + (__FILE__ + (":" + std::to_string(__LINE__))))

MainLoop::MainLoop() :
    irc_message_handler(std::make_shared<IRCBotMessageHandler>()),
    active(false)
{
}

void MainLoop::add_irc_bot(IRCBot::Ptr& bot)
{
    bots.push_front(bot);

    if (!bot->active())
        bot->init_bot();
}

void MainLoop::start()
{
    for (auto& b : bots)
        if (!b->active())
            b->init_bot();

    active = true;
}

bool MainLoop::running() const
{
    return active;
}

void MainLoop::override_message_handler(
    std::function<std::shared_ptr<IRCBotMessageHandler>(void)> const& message_handler_builder)
{
    irc_message_handler = message_handler_builder();
}

void MainLoop::parse_irc_messages(std::string const& valid_messages, IRCBot::Ptr const& bot) const
{
    IRCParser parser(valid_messages);

    // if we hit an error lets attempt to keep going!
    auto irc_msgs = [&parser]() -> IRCMessageQueue {
        try
        {
            return parser.parse();
        }
        catch (std::runtime_error const& err)
        {
            std::cerr << "Parser error: " << std::endl <<
                         "  " << err.what() << std::endl;
            return IRCMessageQueue();
        }
    } ();

    while (!irc_msgs.empty())
    {
        auto irc_msg = irc_msgs.front();
        irc_msgs.pop();

        if (irc_msg.command == "PING")
        {
            Servers servers;
            for (auto const& server : irc_msg.params)
                servers.push_back(server);

            bot->pong(servers);
        }
        else
        {
            // Some servers need the socket to be more connected
            if (irc_msg.command == "001")
                bot->connected();

            irc_message_handler->handle(irc_msg, bot);
        }
    }
}

void MainLoop::read_sockets()
{
    fd_set rfds;
    struct timeval wait;

    wait.tv_sec  = 1;
    wait.tv_usec = 0;

    FD_ZERO(&rfds);
    for (auto const& b : bots)
        FD_SET(b->irc_socket(), &rfds);

    auto max_socket = std::max_element(std::begin(bots), std::end(bots), [](auto const& b1, auto const& b2)
            { return b1->irc_socket() < b2->irc_socket(); });

    int retval = select((*max_socket)->irc_socket() + 1, &rfds, NULL, NULL, &wait);

    if (retval == -1)
    {
        throw std::system_error(errno, std::system_category(),
                                REPORT_ERROR("select failed: "));
    }
    else if (retval)
    {
        std::vector<IRCBot::Ptr> to_remove_bots;
        for (auto const& b : bots)
        {
            if (FD_ISSET(b->irc_socket(), &rfds))
            {
                auto valid_buff = [&b, &to_remove_bots, this]() {
                    try
                    {
                        return irc_socket_reader.read_socket(b->irc_socket());
                    }
                    catch (std::runtime_error const& err)
                    {
                        to_remove_bots.push_back(b);

                        if (to_remove_bots.size() >= bots.size())
                            throw std::runtime_error("All bots connections closed");

                         // Read from any server connections still around
                         return std::string();
                    }
                } ();

                std::cout << valid_buff << std::endl;

                parse_irc_messages(valid_buff, b);
            }
        }

        for (auto& b : to_remove_bots)
            bots.erase(std::remove(std::begin(bots), std::end(bots), b), std::end(bots));
    }
}

} // namespace irc_parser
