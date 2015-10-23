set (IRC_BOT_SRC
     irc_bot.cpp
     irc_bot_message_handler.cpp
     irc_socket_reader.cpp
     main_loop.cpp
     socket.cpp

     main.cpp
    )

set (IRC_BOT_HDR
     irc_bot.h
     irc_bot_message_handler.cpp
     irc_server_info.h
     irc_socket_reader.h
     main_loop.h
     socket.h
    )

include_directories (
                     ${IRC_PARSER_INCLUDE_DIRS}
                     ${CMAKE_BINARY_DIR}
                     ${IRC_PARSER_SOURCE_DIR}/src/
                    )

add_executable (irc_bot ${IRC_BOT_SRC} ${IRC_BOT_HDR})

target_link_libraries (irc_bot irc-parser)
