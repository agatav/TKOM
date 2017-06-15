cmake_minimum_required(VERSION 3.6)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.in ${CMAKE_CURRENT_BINARY_DIR}/lex.yy)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp
        COMMAND ${FLEX_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp ${CMAKE_CURRENT_BINARY_DIR}/lex.yy
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lex.yy
        )
ADD_CUSTOM_TARGET(generate_scanner DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp)

set(SOURCE_FILES
        ${SOURCE_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp

        PARENT_SCOPE
        )

set(HEADER_FILES
        ${HEADER_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/lexeme.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lexer.hpp

        PARENT_SCOPE
        )

set(GENERATED_FILES
        ${GENERATED_FILES}

        ${CMAKE_CURRENT_BINARY_DIR}/lex.yy
        ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp

        PARENT_SCOPE
        )
