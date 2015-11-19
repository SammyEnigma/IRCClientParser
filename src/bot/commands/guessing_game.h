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

#ifndef GUESSING_GAME_H
#define GUESSING_GAME_H

#include <random>

namespace irc_parser
{

enum class GuessResult : unsigned
{
    HIGH,
    LOW,
    CORRECT
};

class GuessingGame
{
public:
    GuessingGame();
    explicit GuessingGame(uint64_t max);

    GuessResult attempt_guess(uint64_t guess);

    void restart();
    void set_max(uint64_t new_max);
    void set_level_increment(int increment);

    void next_level();

    uint64_t guesses() const;
    uint64_t level_on() const;
    uint64_t limit() const;

private:
    uint64_t max{100};

    std::mt19937 mt;

    uint64_t answer{0};
    uint64_t number_of_guesses{0};
    uint64_t level{1};
    int increment{2};
};

} // namespace irc_parser

#endif // GUESSING_GAME_H
