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

#include "irc_socket_reader.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <stdexcept>

namespace irc_parser
{

namespace
{
int const max_buff{80};
char const* connection_closed{"----- Connection has closed -------"};
}

IRCSocketReader::IRCSocketReader()
{
}

std::string IRCSocketReader::read_socket(int sock)
{
    int numbytes{0};
    char buf[max_buff];
    auto valid_buff{unfinished_messages[sock]};

    do
    {
        numbytes = recv(sock, buf, max_buff, 0);
        buf[numbytes] = '\0';
        valid_buff += buf;
    } while (numbytes >= max_buff);

    size_t pos = valid_buff.rfind('\n');
    if (pos + 1 < valid_buff.size())
    {
        unfinished_messages[sock] = valid_buff.substr(pos + 1);
        valid_buff = valid_buff.substr(0, pos + 1);
    }
    else
    {
        unfinished_messages[sock].clear();
    }

    if (numbytes <= 0)
        throw std::runtime_error(connection_closed);

    return valid_buff;
}

} // namespace irc_parser
