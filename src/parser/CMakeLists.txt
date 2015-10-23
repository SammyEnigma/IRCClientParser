set (IRC_PARSER_SRC
     irc_command_parser.cpp
     irc_parser.cpp
     lexer.cpp
     token_stream.cpp
    )

set (IRC_PARSER_HDR
     irc_command_parser.h
     irc_parser.h
     lexer.h
     token_stream.h
    )

set (PREFIXDIR "${CMAKE_INSTALL_PREFIX}")
set (EXEC_PREFIX "${CMAKE_INSTALL_PREFIX}")
set (LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
set (INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")

configure_file (irc-parser.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/irc-parser.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/irc-parser.pc
         DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

add_library (irc-parser SHARED ${IRC_PARSER_SRC} ${IRC_PARSER_HDR})

install (TARGETS irc-parser
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
        )

install (FILES ${IRC_PARSER_HDR}
         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/irc_parser)
