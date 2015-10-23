set (IRC_PARSER_LOADER_SRC
     irc_bot.cpp
     irc_bot_message_handler.cpp
     irc_command_parser.cpp
     irc_parser.cpp
     irc_socket_reader.cpp
     main_loop.cpp
     lexer.cpp
     socket.cpp
     token_stream.cpp

     main.cpp
    )

set (IRC_PARSER_LOADER_HDR
     irc_bot.h
     irc_bot_message_handler.cpp
     irc_command_parser.h
     irc_parser.h
     irc_server_info.h
     irc_socket_reader.h
     lexer.h
     main_loop.h
     socket.h
     token_stream.h
    )

include_directories (${IRC_PARSER_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (irc_parser ${IRC_PARSER_LOADER_SRC} ${IRC_PARSER_LOADER_HDR})

target_link_libraries (irc_parser ${IRC_PARSER_LIBRARIES} ${IRC_PARSER_LDFLAGS})
