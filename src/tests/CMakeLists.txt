cmake_minimum_required(VERSION 3.6)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(util)


set(SOURCE_FILES_TEST ${SOURCE_FILES_TEST} PARENT_SCOPE)
set(HEADER_FILES_TEST ${HEADER_FILES_TEST} PARENT_SCOPE)
