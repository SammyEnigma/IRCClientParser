// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2014 Brandon Schaefer
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

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

#include "main_loop.h"

#include "time.h"


namespace ip = irc_parser;

namespace
{
std::string const bot_name{"SeaB0t"};
}

void start_main_loop()
{
    ip::IRCServerInfo freenode{"irc.freenode.net", "8001", "", bot_name, bot_name + " nothing here :Real Name"};
    ip::IRCBot::Ptr irc_bot = std::make_shared<ip::IRCBot>(freenode);

    ip::IRCServerInfo sym{"irc.imperium.ca", "6667", "", bot_name, bot_name + " nothing here :Real Name"};
    ip::IRCBot::Ptr sym_irc_bot = std::make_shared<ip::IRCBot>(sym);

    ip::MainLoop main_loop;

    try
    {
        irc_bot->init_bot([&irc_bot]() { irc_bot->join_channel({"#webelearningshit"}); });
        main_loop.add_irc_bot(irc_bot);
    }
    catch (std::system_error const& err)
    {
        std::cout << "Failed to connect to the server:\n  " << err.what() << std::endl;
    }

    try
    {
        sym_irc_bot->init_bot([&sym_irc_bot]() { sym_irc_bot->join_channel({"#batcave","#sym-NA","#teamsymthic","#testing"}); });
        main_loop.add_irc_bot(sym_irc_bot);
    }
    catch (std::system_error const& err)
    {
        std::cout << "Failed to connect to the server:\n  " << err.what() << std::endl;
    }

    main_loop.start();
    while (main_loop.running())
        main_loop.read_sockets();
}

int main()
{
    srand(time(nullptr));
    try
    {
        start_main_loop();
    }
    catch (std::system_error const& err)
    {
        std::cerr << "System error: " << std::endl
                  << "  code: " << err.code() << ": " << err.what() << std::endl;
    }
    catch (std::runtime_error const& err)
    {
        std::cerr << "Runtime error: " << std::endl
                 << "  " << err.what() << std::endl;
    }
}
