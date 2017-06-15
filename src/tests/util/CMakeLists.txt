cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES_TEST
        ${SOURCE_FILES_TEST}

        ${CMAKE_CURRENT_SOURCE_DIR}/output-stream-lookup-buffer-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string-output-stream-test.cpp

        PARENT_SCOPE
        )