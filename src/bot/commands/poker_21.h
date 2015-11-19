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

#ifndef POKER_21_H
#define POKER_21_H

#include "deck.h"

namespace irc_parser
{
struct Hand
{
    Card first;
    Card second;
};

class Poker21
{
public:
    Poker21();

    bool playing() const;
    void start_hand();
    void stop();

    Hand dealers_hand() const;
    Hand deal_hand();

private:
    bool started{false};
    Hand dealers;
    Deck deck;
};

}

#endif /* POKER_21_H */
