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

#include "irc_bot.h"
#include "irc_bot_message_handler.h"
#include "parser/irc_command_parser.h"
#include "parser/lexer.h"
#include "parser/token_stream.h"

#include <iostream>
#include <sstream>

namespace irc_parser
{

class PrivMessageData
{
public:
    typedef std::shared_ptr<PrivMessageData> Ptr;

    std::string channel_to_reply;
};

namespace
{
#define REPORT_ERROR(msg) msg + (" @:" + (__FILE__ + (":" + std::to_string(__LINE__))))

int const buf_size{80};
std::string const topic{"#webelearningshit - this is the topic"};

std::tuple<int, std::string> read_popen_command(std::string const& command)
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

struct Command
{
    std::string command;
    std::function<void(IRCCommandMessage const&, IRCBot::Ptr const&, PrivMessageData::Ptr&)> handle;
};

typedef std::vector<Command> Commands;

void google(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
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

        std::cout << "SEACH: " << search << std::endl;

        std::string google_lucky = "curl -s --get --data-urlencode \"q=";
        google_lucky += search;
        google_lucky += "\" http://ajax.googleapis.com/ajax/services/search/web?v=1.0 | sed 's/\"unescapedUrl\":\"\\([^\"]*\\).*/\\1/;s/.*GwebSearch\",//'";

        auto google_ret = read_popen_command(google_lucky);
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

            irc_bot->send_message(data->channel_to_reply, google);
        }
    }
    else
    {
        irc_bot->send_message(data->channel_to_reply, "[ERROR] google expects 1 or more arguments");
    }
}

void youtube(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    if (!irc_cmd_msg.params.empty())
    {
        std::string search;
        for (auto const& p : irc_cmd_msg.params)
            search += p + "+";
        search.pop_back();
        irc_bot->send_message(data->channel_to_reply, "https://www.youtube.com/results?q=" + search);
    }
    else
    {
        irc_bot->send_message(data->channel_to_reply, "[ERROR] youtube expects 1 or more arguments");
    }
}

void weather(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    if (!irc_cmd_msg.params.empty())
    {
        std::string weather("we ");
        for (auto const& p : irc_cmd_msg.params)
            weather += p + "-";
        weather.pop_back();

        auto weather_ret = read_popen_command(weather);
        auto zip_weather = std::get<1>(weather_ret);
        if (!zip_weather.empty())
            irc_bot->send_message(data->channel_to_reply, zip_weather);
    }
    else
    {
        irc_bot->send_message(data->channel_to_reply, "[ERROR] weather expects 1 or more arguments");
    }
}

void fortune(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    auto fortune_ret = read_popen_command("fortune");
    auto fortune = std::get<1>(fortune_ret);
    std::stringstream ss(fortune);
    std::vector<std::string> split_lines;
    std::string split_line;
    while (std::getline(ss, split_line, '\n'))
        if (!split_line.empty())
            split_lines.push_back(split_line);

    if (!split_lines.empty())
        for (auto const& s : split_lines)
            irc_bot->send_message(data->channel_to_reply, s);
}

void pick(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    auto picked = irc_cmd_msg.params[rand() % irc_cmd_msg.params.size()];
    irc_bot->send_message(data->channel_to_reply, picked);
}

void wiki(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    if (irc_cmd_msg.params.empty())
    {
        auto wiki_ret = read_popen_command("curl -Ls -o /dev/null -w %{url_effective} https://en.wikipedia.org/wiki/Special:Random");
        auto wiki = std::get<1>(wiki_ret);
        if (!wiki.empty())
        {
            wiki = replace_symbols_in_url(wiki);
            irc_bot->send_message(data->channel_to_reply, wiki);
        }
    }
    else
    {
        std::string search;
        for (auto const& p : irc_cmd_msg.params)
            search += p + "_";
        search.pop_back();
        auto wiki_ret = read_popen_command("curl -Ls -o /dev/null -w %{url_effective} https://en.wikipedia.org/wiki/" + search);
        auto wiki = std::get<1>(wiki_ret);
        if (!wiki.empty())
        {
            wiki = replace_symbols_in_url(wiki);
            irc_bot->send_message(data->channel_to_reply, wiki);
        }
    }
}

void fact(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    auto fact_ret = read_popen_command("wget randomfunfacts.com -O - 2>/dev/null | grep \\<strong\\> | sed \"s;^.*<i>\\(.*\\)</i>.*$;\\1;\"");
    auto fact = std::get<1>(fact_ret);
    if (!fact.empty())
        irc_bot->send_message(data->channel_to_reply, fact);
}

void ghc(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    std::string ghc_cmd("ghc -e '");
    for (auto const& p : irc_cmd_msg.params)
        ghc_cmd += p + " ";
    ghc_cmd.pop_back();
    ghc_cmd += "'";

    auto ghc_ret = read_popen_command(ghc_cmd);
    auto ghc = std::get<1>(ghc_ret);
    std::stringstream ss(ghc);
    std::vector<std::string> split_lines;
    std::string split_line;
    while (std::getline(ss, split_line, '\n'))
    if (!split_line.empty())
        split_lines.push_back(split_line);

    if (!split_lines.empty())
        for (auto const& line : split_lines)
            irc_bot->send_message(data->channel_to_reply, line);
}

void python(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    std::string python_cmd("python -c \"exec(\\\"import math\\\\nprint ");
    for (auto const& p : irc_cmd_msg.params)
        python_cmd += p + " ";
    python_cmd.pop_back();
    python_cmd += "\\\")\"";

    auto python_ret = read_popen_command(python_cmd);
    auto python = std::get<1>(python_ret);
    std::stringstream ss(python);
    std::vector<std::string> split_lines;
    std::string split_line;
    while (std::getline(ss, split_line, '\n'))
    if (!split_line.empty())
        split_lines.push_back(split_line);

    if (!split_lines.empty())
        for (auto const& line : split_lines)
            irc_bot->send_message(data->channel_to_reply, line);
}

void source(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    irc_bot->send_message(data->channel_to_reply, "https://github.com/BrandonSchaefer/IRCClientParser");
}

void help(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data);

Commands commands{
    {"google", google},
    {"youtube", youtube},
    {"weather", weather},
    {"fortune", fortune},
    {"pick", pick},
    {"wiki", wiki},
    {"fact", fact},
    {"ghc", ghc},
    {"python", python},
    {"source", source},
    {"help", help},
};

void help(IRCCommandMessage const& irc_cmd_msg, IRCBot::Ptr const& irc_bot, PrivMessageData::Ptr& data)
{
    std::string help;
    for (auto const& c : commands)
        help += "!" + c.command + " ";
     help.pop_back();

     irc_bot->send_message(data->channel_to_reply, help);
}

}

IRCBotMessageHandler::IRCBotMessageHandler() :
    priv_message_data(std::make_shared<PrivMessageData>())
{
}

void IRCBotMessageHandler::handle(IRCMessage const& irc_msg, IRCBot::Ptr const& irc_bot)
{
    if (irc_msg.command == "PRIVMSG")
    {
        IRCCommandParser command_parser(irc_msg.params[1]);
        auto irc_cmd_msg = [&command_parser, &irc_bot]() -> IRCCommandMessage {
            try
            {
                return command_parser.parse(irc_bot->name());
            }
            catch (std::runtime_error const& err)
            {
                //ignore expect error if text is not a command
                return {};
            }
        } ();

        auto channel_to_reply = irc_msg.params[0];
        if (channel_to_reply == irc_bot->name())
            channel_to_reply = irc_msg.prefix.nick;

        // remove me later
        priv_message_data->channel_to_reply = channel_to_reply;

        for (auto const& c : commands)
        {
            if (irc_cmd_msg.command == c.command)
            {
                c.handle(irc_cmd_msg, irc_bot, priv_message_data);
                return;
            }
        }

        if (irc_msg.params[1].find(irc_bot->name()) != std::string::npos || irc_msg.params[0] == irc_bot->name())
        {

            static std::vector<std::string> l = {"what do you want?",
                                                 "Thanks",
                                                 "I don't care.",
                                                 "I dont give a rats ass",
                                                 "The dog wants out",
                                                 "I DONT KNOW",
                                                 "That is really unique",
                                                 "I couldn't care less",
                                                 "Shit fuck shit loop",
                                                 "two plus two is not what i know",
                                                 "you are stupid",
                                                 "Hmmm I think ill repeat my self now",
                                                 "This isnt going to end well...",
                                                 "Now is the time to stop",
                                                 "Could you no do that?",
                                                 "Why yes, that is not correct"
                                                };

            irc_bot->send_message(channel_to_reply, {irc_msg.prefix.nick + ", " + l[rand() % l.size()]});
        }
        else if (!irc_cmd_msg.command.empty())
        {
            auto cmd = "COMMAND: " + irc_cmd_msg.command;
            if (!irc_cmd_msg.params.empty())
            {
                cmd += " PARAMS: ";

                for (auto const& p : irc_cmd_msg.params)
                    cmd += p + " ";
                cmd.pop_back();
            }

            irc_bot->send_message(channel_to_reply, cmd);
        }
    }
    else if (irc_msg.command == "JOIN")
    {
        if (irc_msg.prefix.nick != irc_bot->name() && irc_msg.params[0] == "#webelearningshit")
            irc_bot->send_message(irc_msg.params[0], {"Welcome, " + irc_msg.prefix.nick});
    }
    //else if (irc_msg.command == "332")
    //{
        //irc_bot->send_message(irc_msg.params[1], irc_msg.params[2]);
    //}
    else if (irc_msg.command == "TOPIC")
    {
        //irc_bot->send_message(irc_msg.params[0], irc_msg.params[1]);
        //if (irc_msg.params[0] == "#webelearningshit" && irc_msg.prefix.nick != irc_bot->name())
            //irc_bot->send_to_server("TOPIC " + irc_msg.params[0] + " :" + topic + "\r\n");
    }
}

} // namespace irc_parser
