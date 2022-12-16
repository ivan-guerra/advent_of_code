cmake_minimum_required(VERSION 3.13...3.22)

project(advent_of_code_2022
    VERSION 1.0
    DESCRIPTION "Advent of Code 2022 Solutions"
)

set(AOC_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING "${PROJECT_NAME} binary directory.")


add_subdirectory(src)
