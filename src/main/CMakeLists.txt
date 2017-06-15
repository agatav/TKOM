cmake_minimum_required(VERSION 3.6)

add_subdirectory(lexer)
add_subdirectory(util)
add_subdirectory(logging)
add_subdirectory(parser)
add_subdirectory(interpreter)


set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)

set(GENERATED_FILES ${GENERATED_FILES} PARENT_SCOPE)
