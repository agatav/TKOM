cmake_minimum_required(VERSION 3.6)

set(HEADER_FILES
        ${HEADER_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/node.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/node-visitor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp

        PARENT_SCOPE
        )

set(SOURCE_FILES
        ${SOURCE_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp

        PARENT_SCOPE
        )
