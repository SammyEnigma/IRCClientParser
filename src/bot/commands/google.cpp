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

#include "google.h"
#include "popen_command.h"

namespace irc_parser
{

namespace
{
std::string const google_match{"google"};
std::string const google_search_prefix{"curl -s --get --data-urlencode \"q="};
std::string const google_search_postfix{
    "\" http://ajax.googleapis.com/ajax/services/search/web?v=1.0 | sed 's/\"unescapedUrl\":\"\\([^\"]*\\).*/\\1/;s/.*GwebSearch\",//'"};

std::string replace_symbols_in_url(std::string const& url)
{
    // Replace ( with %28 and ) with %29
    std::string replaced;
    for (auto const& c : url)
    {
        if (c == '(')
        {
            replaced += "%28";
        }
        else if (c == ')')
        {
            replaced += "%29";
        }
        else
        {
            replaced += c;
        }
    }

    return replaced;
}
}

void Google::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{
    if (!irc_cmd_msg.params.empty())
    {
        std::string search;
        for (auto const& p : irc_cmd_msg.params)
        {
            std::string replace_plus;
            for (auto const& s : p)
            {
                if (s == '+')
                    replace_plus += "%2B";
                else
                    replace_plus += s;
            }
            search += replace_plus + "+";
        }
        search.pop_back();

        std::string google_lucky = google_search_prefix;
        google_lucky += search;
        google_lucky += google_search_postfix;

        auto google_ret = exec_popen_command(google_lucky);
        auto google = std::get<1>(google_ret);
        if (!google.empty())
        {
            size_t pos = google.find("\\u003d");
            // need to replace \u003 with '='
            if (pos != std::string::npos)
            {
                std::string temp = google.substr(0, pos);
                temp += '=';
                temp += google.substr(pos + 6);
                google = temp;
            }

            google = replace_symbols_in_url(google);

            irc_bot->send_message(channel_to_reply, google);
        }
    }
    else
    {
        irc_bot->send_message(channel_to_reply, "[ERROR] google expects 1 or more arguments");
    }
}

std::vector<std::string> Google::match() const
{
    return {google_match};
}

}
