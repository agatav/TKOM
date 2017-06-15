cmake_minimum_required(VERSION 3.6)
project(tkom)

set(CMAKE_CXX_STANDARD 14)

find_package(FLEX REQUIRED)
find_package(GTest REQUIRED)
find_package (Threads)

add_subdirectory(src)
SET_SOURCE_FILES_PROPERTIES(${GENERATED_FILES} PROPERTIES GENERATED 1)

add_executable(runner ${SOURCE_FILES} ${HEADER_FILES} main.cpp)
add_dependencies(runner generate_scanner)
target_include_directories(runner PUBLIC ${FLEX_INCLUDE_DIRS})

enable_testing()
add_executable(test-all
               ${SOURCE_FILES} ${HEADER_FILES}
               ${SOURCE_FILES_TEST} ${HEADER_FILES_TEST}
               main-test.cpp
               )
target_include_directories(test-all PUBLIC src/main ${FLEX_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
target_link_libraries(test-all ${GTEST_LIBRARIES})
target_link_libraries (test-all ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(test-all generate_scanner)
add_test(runUnitTests test-all)
