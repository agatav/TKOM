cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES_TEST
        ${SOURCE_FILES_TEST}

        ${CMAKE_CURRENT_SOURCE_DIR}/fake-visitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/parser-test.cpp

        PARENT_SCOPE
        )

set(HEADER_FILES_TEST
        ${HEADER_FILES_TEST}

        ${CMAKE_CURRENT_SOURCE_DIR}/fake-visitor.hpp

        PARENT_SCOPE
        )