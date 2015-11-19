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

#include "popen_command.h"

#include <stdio.h>
#include <system_error>

namespace irc_parser
{
namespace
{
#define REPORT_ERROR(msg) msg + (" @:" + (__FILE__ + (":" + std::to_string(__LINE__))))
int const buf_size{80};
}

std::tuple<int, std::string> exec_popen_command(std::string const& command)
{
    //FILE* p = popen(std::string(command + " 2>&1").c_str(), "r");
    FILE* p = popen(command.c_str(), "r");

    if (!p)
        throw std::system_error(errno, std::system_category(),
                                REPORT_ERROR("popen has failed"));

    std::string p_output;
    char buf[buf_size];
    size_t bytes_read = buf_size;

    while (bytes_read)
    {
        bytes_read = fread(buf, sizeof(char), buf_size, p);
        buf[bytes_read] = '\0';
        p_output += buf;
    }

    int ret = pclose(p);
    if(WIFEXITED(ret))
        ret = WEXITSTATUS(ret);

    return std::tie(ret, p_output);
}

} // namespace irc_parser

