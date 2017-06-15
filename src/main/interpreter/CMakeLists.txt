set(HEADER_FILES
        ${HEADER_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/interpreter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scope.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/variable.hpp

        PARENT_SCOPE
        )

set(SOURCE_FILES
        ${SOURCE_FILES}

        ${CMAKE_CURRENT_SOURCE_DIR}/interpreter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scope.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/variable.cpp

        PARENT_SCOPE
        )