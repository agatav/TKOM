cmake_minimum_required(VERSION 3.6)

set(HEADER_FILES
        ${HEADER_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/generic-exception.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/output-stream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/output-stream-lookup-buffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stdin-output-stream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string-output-stream.hpp

        PARENT_SCOPE
        )

set(SOURCE_FILES
        ${SOURCE_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/stdin-output-stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string-output-stream.cpp

        PARENT_SCOPE
        )