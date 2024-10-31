cmake_minimum_required(VERSION 3.0)

project(roslang VERSION 1.0)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(BISON REQUIRED)
if (BISON_FOUND)
    BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/../parser.cpp)

    include_directories(${CMAKE_CURRENT_BINARY_DIR})

endif()

find_package(FLEX REQUIRED)

if (FLEX_FOUND)
    FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/../lexer.cpp)

    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

include_directories(${PROJECT_SOURCE_DIR}/*.hpp)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(roslang ${SOURCES} ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS})