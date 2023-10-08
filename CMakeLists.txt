cmake_minimum_required(VERSION 3.13)

project(advent_of_code
    VERSION 1.0
    DESCRIPTION "Advent of Code Solutions"
)

set(AOC_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING "${PROJECT_NAME} binary directory.")

add_subdirectory(src)
