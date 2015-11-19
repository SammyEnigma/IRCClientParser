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

#ifndef DECK_H
#define DECK_H

#include <random>

namespace irc_parser
{
struct Card
{
    enum class Suit : unsigned
    {
        HEART,
        SPADE,
        CLUB,
        DIAMOND,
        Size
    };

    enum class Value : unsigned
    {
        ACE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        Size
    };

    Suit  suit;
    Value value;
};

// TODO Look at making this base class then make this end, and have an infinte deck
// TODO infinte for right now
class Deck
{
public:
    Deck();

    bool has_next() const;
    size_t size() const;

    Card next_card();

    void shuffle();

private:
    void fill();

    std::mt19937 mt;
    std::vector<Card> deck;
};

}

#endif // DECK_H
