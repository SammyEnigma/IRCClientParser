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

#include "deck.h"

namespace irc_parser
{

Deck::Deck() :
    mt(std::random_device()())
{
    shuffle();
}

bool Deck::has_next() const
{
    return !deck.empty();
}

size_t Deck::size() const
{
    return deck.size();
}

Card Deck::next_card()
{
    auto card = deck.back();
    deck.pop_back();
    // new deck
    if (deck.empty())
        shuffle();

    return card;
}

void Deck::fill()
{
    deck.clear();

    auto suit_size  = static_cast<unsigned>(Card::Suit::Size);
    auto value_size = static_cast<unsigned>(Card::Value::Size);
    for (unsigned suits = 0; suits < suit_size; suits++)
    {
        auto new_suit = static_cast<Card::Suit>(suits);
        for (unsigned value = 0; value < value_size; value++)
        {
            auto new_value = static_cast<Card::Value>(value);
            deck.push_back({new_suit, new_value});
        }
    }
}

// Fisher–Yates shuffle
void Deck::shuffle()
{
    fill();

    unsigned left_to_shuffle{deck.size() - 1};

    for (unsigned i = left_to_shuffle; i > 0; i--)
    {
        std::uniform_int_distribution<unsigned> dist(0, i);
        auto index_to_shuffle = dist(mt);

        auto temp_card         = std::move(deck[index_to_shuffle]);
        deck[index_to_shuffle] = std::move(deck[i]);
        deck[i]                = std::move(temp_card);
    }
}


}
