set (IRC_BOT_SRC
     irc_bot.cpp
     irc_bot_message_handler.cpp
     irc_socket_reader.cpp
     main_loop.cpp
     socket.cpp

     commands/guessing_game.cpp
     commands/guessing_game_policy.cpp
     commands/deck.cpp
     commands/poker_21.cpp
     commands/poker_21_policy.cpp
     commands/google.cpp
     commands/youtube.cpp
     commands/wiki.cpp
     commands/popen_command.cpp
     commands/weather.cpp
     commands/fortune.cpp
     commands/pick.cpp
     commands/fact.cpp
     commands/ghc.cpp
     commands/python.cpp

     main.cpp
    )

set (IRC_BOT_HDR
     irc_bot.h
     irc_bot_message_handler.cpp
     irc_server_info.h
     irc_socket_reader.h
     main_loop.h
     socket.h

     commands/command.h
     commands/deck.h
     commands/guessing_game.h
     commands/guessing_game_policy.h
     commands/deck.h
     commands/poker_21.h
     commands/poker_21_policy.h
     commands/google.h
     commands/youtube.h
     commands/wiki.h
     commands/popen_command.h
     commands/weather.h
     commands/fortune.h
     commands/pick.h
     commands/fact.h
     commands/ghc.h
     commands/python.h
    )

include_directories (
                     ${IRC_PARSER_INCLUDE_DIRS}
                     ${CMAKE_BINARY_DIR}
                     ${IRC_PARSER_SOURCE_DIR}/src/
                    )

add_executable (irc_bot ${IRC_BOT_SRC} ${IRC_BOT_HDR})

target_link_libraries (irc_bot irc-parser)
