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

#ifndef SOCKET_H
#define SOCKET_H

#include "irc_server_info.h"

#include <string>

typedef struct addrinfo addrinfo;

namespace irc_parser
{

class Socket
{
public:
    explicit Socket(IRCServerInfo const& server_info);
    ~Socket();

    void create_and_connect();
    int operator()() const;
    bool operator<(Socket const& s) const;

    void send_to_socket(std::string const& msg) const;

    bool created() const;

protected:
    Socket();

private:
    int get_addr_info(addrinfo** serverinfo);
    void create_socket(addrinfo* serverinfo);
    void connect_socket(addrinfo* serverinfo);
    int sock{0};
    IRCServerInfo server_info;
};

} // namespace irc_parser

#endif // SOCKET_H
