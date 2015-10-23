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

#include "irc_parser.h"
#include "token_stream.h"

#include <stdexcept>

namespace irc_parser
{

namespace
{
}

IRCParser::IRCParser(std::string const& text) :
    token_stream(text)
{
}

// message    =  [ ":" prefix SPACE ] command [ params ] crlf
IRCMessageQueue IRCParser::parse()
{
    std::queue<IRCMessage> irc_messages;

    while (token_stream.has_next())
    {
        auto t = token_stream.look();
        IRCMessage irc_message;

        // [ ":" prefix SPACE ]
        if (t.type == Type::COLON)
        {
            t = token_stream.get_next();
            auto prefix = parse_prefix();
            t = token_stream.look();

            irc_message.prefix = std::move(prefix);
        }

        // command
        auto command = parse_command();
        irc_message.command = std::move(command);

        t = token_stream.look();

        // [ params ]
        if (t.type == Type::SPACE)
        {
            token_stream.next();
            auto params = parse_params();
            if (token_stream.has_next())
                t = token_stream.look();

            irc_message.params = std::move(params);
        }

        // crlf
        token_stream.while_has({Type::CR, Type::LF});

        irc_messages.push(irc_message);
    }

    return irc_messages;
}

// shortname  =  ( letter / digit ) *( letter / digit / "-" )
//                 *( letter / digit ) // ISSUE WITH THE RFC :(
std::string IRCParser::parse_shortname()
{
    auto t = token_stream.look();
    std::string shortname;

    // shortname *( "." shortname )
    do
    {
        // ( letter / digit )
        shortname += t.token;
        t = token_stream.get_next();

        // *( letter / digit / "-" )
        // FIXME For some reason some servers think its ok to use a '/' in the server/hostname
        shortname += token_stream.while_has({Type::LETTER, Type::DIGIT, Type::DASH, Type::FSLASH});
        t = token_stream.look();
    } while (t.type == Type::DOT);

    return shortname;
}

std::string IRCParser::parse_nick()
{
    // *8( letter / digit / special / "-" )
    return token_stream.while_has({Type::LETTER, Type::DIGIT, Type::SPECIAL, Type::DASH});
}

std::string IRCParser::parse_user()
{
    // user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
    //                ; any octet except NUL, CR, LF, " " and "@"
    return token_stream.while_has_not({Type::NUL, Type::CR, Type::LF, Type::SPACE, Type::AT});
}

std::string IRCParser::parse_host()
{
    auto host = parse_shortname();
    auto t = token_stream.look();

    // ip6addr    =  1*hexdigit 7( ":" 1*hexdigit )
    // ip6addr    =/ "0:0:0:0:0:" ( "0" / "FFFF" ) ":" ip4addr
    if (t.type == Type::COLON)
    {
        host += token_stream.while_has({Type::COLON, Type::LETTER, Type::DIGIT, Type::DOT});
        t = token_stream.look();
    }

    return host;
}

Prefix IRCParser::parse_prefix()
{
    Prefix prefix;
    auto t = token_stream.look();
    // hostname   =  shortname *( "." shortname )
    if (t.type == Type::LETTER ||
        t.type == Type::DIGIT)
    {
        prefix.server = parse_shortname();
        t = token_stream.look();
    }

    // We could actually be a nickname
    if (t.type != Type::SPACE)
    {
        // ( nickname [ [ "!" user ] "@" host ] )
        // nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
        if (t.type == Type::LETTER ||
            t.type == Type::SPECIAL ||
            (t.type == Type::DASH &&
            !prefix.server.empty()))
        {
            prefix.nick = prefix.server + parse_nick();
            prefix.server.clear();
            t = token_stream.look();
        }
        // Hack, if servername doesnt have a '.' assume nick
        else
        {
            if (prefix.server.find('.') == std::string::npos)
            {
                prefix.nick = prefix.server;
                prefix.server.clear();
            }
        }

        // We read the username
        // [ "!" user ]
        if (t.type == Type::BANG)
        {
            token_stream.next();
            prefix.user = parse_user();
            t = token_stream.look();
        }

        // We read the username
        // [ "@" host ]
        if (t.type == Type::AT)
        {
            token_stream.next();
            prefix.host = parse_host();
            t = token_stream.look();
        }
    }

    if (t.type != Type::SPACE)
        throw std::runtime_error("Space expected, token: " + t.token +
                                 " type: " + token_type_name(t.type));

    // eat space
    token_stream.next();

    return prefix;
}

std::string IRCParser::parse_command()
{
    // command    =  1*letter / 3digit
    std::string command;
    auto t = token_stream.look();

    if (t.type == Type::LETTER)
    {
        command = t.token;
        t = token_stream.get_next();
    }
    else if (t.type == Type::DIGIT && t.token.size() <= 3)
    {
        if (t.token.size() <= 3)
        {
            command = t.token;
            t = token_stream.get_next();
        }
        else
        {
            throw std::runtime_error("Command length to long, no more then 3, Token: " +
                                     t.token + " type: " + token_type_name(t.type));
        }
    }
    else
    {
        throw std::runtime_error("Command must start with a Letter or a Digit Token: " +
                                 t.token + " type: " + token_type_name(t.type));
    }

    return command;
}

// trailing   =  *( ":" / " " / nospcrlfcl )
std::string IRCParser::parse_trailing()
{
    auto t = token_stream.look();

    if (t.type == Type::COLON)
        t = token_stream.get_next();

    return token_stream.while_has_not({Type::NUL, Type::CR, Type::LF});
}

// params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
//            =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]
std::vector<std::string> IRCParser::parse_params()
{
    auto t = token_stream.look();
    std::vector<std::string> params;

    // nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
    //               ; any octet except NUL, CR, LF, " " and ":"
    // middle     =  nospcrlfcl *( ":" / nospcrlfcl )
    while (t.type != Type::COLON && t.type != Type::CR && t.type != Type::LF)
    {
        auto param = token_stream.while_has_not({Type::NUL, Type::CR, Type::LF, Type::SPACE});
        params.push_back(param);
        t = token_stream.get_next();
    }

    if (t.type == Type::COLON)
    {
        token_stream.next();
        params.push_back(parse_trailing());
    }

     return params;
}

} // namespace irc_parser
