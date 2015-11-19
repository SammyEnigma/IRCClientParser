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

#include "guessing_game.h"

#include <iostream>

namespace irc_parser
{

GuessingGame::GuessingGame() :
    mt(std::random_device()())
{
    restart();
}

GuessingGame::GuessingGame(uint64_t max) :
    max(max),
    mt(std::random_device()())
{
    restart();
}

GuessResult GuessingGame::attempt_guess(uint64_t guess)
{
    number_of_guesses++;

    if (guess < answer)
        return GuessResult::LOW;
    else if (guess > answer)
        return GuessResult::HIGH;

    return GuessResult::CORRECT;
}

void GuessingGame::restart()
{
    std::uniform_int_distribution<uint64_t> dist(1, max);
    answer = dist(mt);
    number_of_guesses = 0;
    level = 1;
}

void GuessingGame::set_max(uint64_t new_max)
{
    max = new_max;
}

void GuessingGame::set_level_increment(int incr)
{
    increment = incr;
}

void GuessingGame::next_level()
{
    std::uniform_int_distribution<uint64_t> dist(1, max);
    max *= increment;
    answer = dist(mt);
    level++;

    std::cout << "HERE: " << answer << " " << max << std::endl;
}

uint64_t GuessingGame::guesses() const
{
    return number_of_guesses;
}

uint64_t GuessingGame::level_on() const
{
    return level;
}

uint64_t GuessingGame::limit() const
{
    return max;
}

} // namespace irc_parser
