cmake_minimum_required(VERSION 3.30)

project(roslang VERSION 1.0)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(BISON REQUIRED)
if (BISON_FOUND)
    BISON_TARGET(Parser ${PROJECT_SOURCE_DIR}/src/parser.y ${PROJECT_SOURCE_DIR}/src/parser.cpp DEFINES_FILE ${PROJECT_SOURCE_DIR}/include/parser.hpp)

endif()

find_package(FLEX REQUIRED)
if (FLEX_FOUND)
    FLEX_TARGET(Lexer ${PROJECT_SOURCE_DIR}/src/lexer.l ${PROJECT_SOURCE_DIR}/src/lexer.cpp)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(roslang main.cpp ${SOURCES} ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS})