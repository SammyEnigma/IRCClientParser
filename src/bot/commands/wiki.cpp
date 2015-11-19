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

#include "wiki.h"
#include "popen_command.h"

namespace irc_parser
{

namespace
{
    std::string const wiki_match{"wiki"};
    std::string const random_wiki_curl_command{
        "curl -Ls -o /dev/null -w %{url_effective} https://en.wikipedia.org/wiki/Special:Random"};
    std::string const wiki_search_curl_command{
        "curl -Ls -o /dev/null -w %{url_effective} https://en.wikipedia.org/wiki/"};

// TODO make a file for this
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

void Wiki::command(
    IRCCommandMessage const& irc_cmd_msg,
    IRCBot::Ptr const& irc_bot,
    std::string const& channel_to_reply)
{

    if (irc_cmd_msg.params.empty())
    {
        auto wiki_ret = exec_popen_command(random_wiki_curl_command);
        auto wiki = std::get<1>(wiki_ret);
        if (!wiki.empty())
        {
            wiki = replace_symbols_in_url(wiki);
            irc_bot->send_message(channel_to_reply, wiki);
        }
    }
    else
    {
        std::string search;
        for (auto const& p : irc_cmd_msg.params)
            search += p + "_";
        search.pop_back();
        auto wiki_ret = exec_popen_command(wiki_search_curl_command + search);
        auto wiki = std::get<1>(wiki_ret);
        if (!wiki.empty())
        {
            wiki = replace_symbols_in_url(wiki);
            irc_bot->send_message(channel_to_reply, wiki);
        }
    }
}


std::vector<std::string> Wiki::match() const
{
    return {wiki_match};
}

}
