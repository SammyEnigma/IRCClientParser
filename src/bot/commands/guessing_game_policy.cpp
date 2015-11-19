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

#include "guessing_game_policy.h"
#include "popen_command.h"

namespace irc_parser
{

namespace
{
std::string const guessing_game_match{"guess"};
}

void GuessingGamePolicy::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    GuessingGame* gg = &gg_per_channel[channel_to_reply];

    bool print_stats = false;
    if (!irc_cmd_msg.params.empty())
    {
        auto guess = [&irc_cmd_msg]() -> uint64_t {
            try
            {
                return std::stoi(irc_cmd_msg.params[0]);
            }
            catch (std::invalid_argument const& /* err */)
            {
                return 0;
            }
        } ();

        if (guess)
        {
            GuessResult result = gg->attempt_guess(guess);
            std::string output;

            if (result == GuessResult::LOW)
            {
                output = "Guess was to low";
            }
            else if (result == GuessResult::HIGH)
            {
                output = "Guess was to high";
            }
            else
            {
                gg->next_level();
                output = "Congrats, correct guess! Next level: " + std::to_string(gg->level_on());
            }

            irc_bot->send_message(channel_to_reply, output);
        }
        else
        {
            print_stats = true;
        }
    }
    else
    {
        print_stats = true;
    }

    if (print_stats)
    {
        auto level = gg->level_on();
        auto n_guesses = gg->guesses();
        auto max = gg->limit();

        irc_bot->send_message(channel_to_reply,
            "Level: " + std::to_string(level) +
            " -  Number of guesses: " + std::to_string(n_guesses) +
            " -  Range: 1 - " + std::to_string(max));
    }
}

std::vector<std::string> GuessingGamePolicy::match() const
{
    return {guessing_game_match};
}

}
