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

#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <system_error>

namespace irc_parser
{

#define REPORT_ERROR(msg) msg + (" @:" + (__FILE__ + (":" + std::to_string(__LINE__))))

Socket::Socket(IRCServerInfo const& server_info) :
    server_info(server_info)
{
}

Socket::~Socket()
{
    close(sock);
}

void Socket::create_and_connect()
{
    addrinfo* serverinfo;
    int res;

    if ((res = get_addr_info(&serverinfo)) != 0)
    {
        throw std::system_error(errno, std::system_category(),
                                REPORT_ERROR("getaddrinfo failure: " +
                                std::string(gai_strerror(res))));
    }

    try
    {
        create_socket(serverinfo);
        connect_socket(serverinfo);
    }
    catch (std::system_error const& err)
    {
        freeaddrinfo(serverinfo);
        close(sock);
        throw err;
    }

    freeaddrinfo(serverinfo);

    if (!server_info.password.empty())
        send_to_socket("PASS " + server_info.password + "\r\n");

    send_to_socket("NICK " + server_info.nick + "\r\n");
    send_to_socket("USER " + server_info.user + "\r\n");
}

int Socket::get_addr_info(addrinfo** serverinfo)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    return getaddrinfo(server_info.server_name.c_str(), server_info.port.c_str(), &hints, serverinfo);
}

void Socket::create_socket(addrinfo* serverinfo)
{
    if ((sock = socket(serverinfo->ai_family,
                       serverinfo->ai_socktype,
                       serverinfo->ai_protocol)) == -1)
    {
        throw std::system_error(errno, std::system_category(),
                                REPORT_ERROR("creating socket failure: "));
    }
}

void Socket::connect_socket(addrinfo* serverinfo)
{
    if (connect(sock, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1)
        throw std::system_error(errno, std::system_category(),
                                REPORT_ERROR("connecting socket failure: "));
}

void Socket::send_to_socket(std::string const& msg) const
{
    send(sock, msg.c_str(), msg.size(), 0);
}

bool Socket::created() const
{
    return sock != 0;
}

int Socket::operator()() const
{
    return sock;
}

bool Socket::operator<(Socket const& s) const
{
    return sock < s.sock;
}

} // namespace irc_parser
