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
#include "irc_bot_message_handler.h"
#include "commands/poker_21_policy.h"
#include "commands/guessing_game_policy.h"
#include "commands/google.h"
#include "commands/youtube.h"
#include "commands/wiki.h"
#include "commands/fact.h"
#include "commands/ghc.h"
#include "commands/pick.h"
#include "commands/python.h"
#include "commands/fortune.h"
#include "commands/weather.h"

#include "parser/irc_command_parser.h"
#include "parser/lexer.h"
#include "parser/token_stream.h"

#include <algorithm>
#include <iostream>

namespace irc_parser
{

namespace
{

/*
void poker(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    auto* poker = &data->poker_game[data->channel_to_reply];
    if (!irc_cmd_msg.params.empty() && irc_cmd_msg.params[0] == "play")
    {
        poker->start_hand();
        auto dealers_hand = poker->dealers_hand();
        irc_bot->send_message(data->channel_to_reply, card_output(dealers_hand.first) + " |x x|");
    }
    else if (!irc_cmd_msg.params.empty() && irc_cmd_msg.params[0] == "stop")
    {
        poker->stop();
    }
    else if (irc_cmd_msg.params.empty() && poker->playing())
    {
        auto players_hand = poker->deal_hand();
        irc_bot->send_message(data->channel_to_reply, card_output(players_hand.first) + "" + card_output(players_hand.second) + " = " + hand_total(players_hand));
    }
}
*/

}

IRCBotMessageHandler::IRCBotMessageHandler()
{
    //TODO make a factory here
    //commands.push_back(std::make_shared<Poker21Policy>());
    commands.push_back(std::make_shared<GuessingGamePolicy>());
    commands.push_back(std::make_shared<Wiki>());
    commands.push_back(std::make_shared<Google>());
    commands.push_back(std::make_shared<Youtube>());
    commands.push_back(std::make_shared<Fact>());
    commands.push_back(std::make_shared<GHC>());
    commands.push_back(std::make_shared<Pick>());
    commands.push_back(std::make_shared<Python>());
    commands.push_back(std::make_shared<Fortune>());
    commands.push_back(std::make_shared<Weather>());
}

void IRCBotMessageHandler::handle(IRCMessage const& irc_msg, IRCBot::Ptr const& irc_bot)
{
    if (irc_msg.command == "PRIVMSG")
    {
        IRCCommandParser command_parser(irc_msg.params[1]);
        auto irc_cmd_msg = [&command_parser, &irc_bot]() -> IRCCommandMessage {
            try
            {
                return command_parser.parse(irc_bot->name());
            }
            catch (std::runtime_error const& err)
            {
                //ignore expect error if text is not a command
                return {};
            }
        } ();

        auto channel_to_reply = irc_msg.params[0];
        if (channel_to_reply == irc_bot->name())
            channel_to_reply = irc_msg.prefix.nick;

        irc_cmd_msg.user = irc_msg.prefix.nick;

        for (auto const& c : commands)
        {
            auto matches = c->match();
            if (std::find(matches.begin(), matches.end(), irc_cmd_msg.command) != matches.end())
            {
                c->command(irc_cmd_msg, irc_bot, channel_to_reply);
                return;
            }
        }

        if (irc_cmd_msg.command == "source")
        {
            irc_bot->send_message(channel_to_reply, "\x03\x31\x02https://github.com/BrandonSchaefer/IRCClientParser\x03");
        }
        else if (irc_cmd_msg.command == "help")
        {
            std::string help;
            for (auto const& c : commands)
            {
                for (auto const& m : c->match())
                {
                    help += "!" + m + " ";
                }
            }
            help.pop_back();

            irc_bot->send_message(channel_to_reply, help);
        }
        else if (irc_msg.params[1].find(irc_bot->name()) != std::string::npos || irc_msg.params[0] == irc_bot->name())
        {

            static std::vector<std::string> l = {"what do you want?",
                                                 "Thanks",
                                                 "I don't care.",
                                                 "I dont give a rats ass",
                                                 "The dog wants out",
                                                 "I DONT KNOW",
                                                 "That is really unique",
                                                 "I couldn't care less",
                                                 "Shit fuck shit loop",
                                                 "two plus two is not what i know",
                                                 "you are stupid",
                                                 "Hmmm I think ill repeat my self now",
                                                 "This isnt going to end well...",
                                                 "Now is the time to stop",
                                                 "Could you no do that?",
                                                 "Why yes, that is not correct"
                                                };

            irc_bot->send_message(channel_to_reply, {irc_msg.prefix.nick + ", " + l[rand() % l.size()]});
        }
        else if (!irc_cmd_msg.command.empty())
        {
            auto cmd = "COMMAND: " + irc_cmd_msg.command;
            if (!irc_cmd_msg.params.empty())
            {
                cmd += " PARAMS: ";

                for (auto const& p : irc_cmd_msg.params)
                    cmd += p + " ";
                cmd.pop_back();
            }

            irc_bot->send_message(channel_to_reply, cmd);
        }
    }
    else if (irc_msg.command == "JOIN")
    {
        if (irc_msg.prefix.nick != irc_bot->name() && irc_msg.params[0] == "#webelearningshit")
            irc_bot->send_notice(irc_msg.prefix.nick, "Welcome, " + irc_msg.prefix.nick);
    }
}

} // namespace irc_parser
