cmake_minimum_required(VERSION 3.13)

project(
  aoc_${AOC_YEAR}_day_11
  DESCRIPTION "AOC 2021 - Day 11 Solutions"
  LANGUAGES CXX)

set(TARGET_NAMES aoc_2021_11_1 aoc_2021_11_2)

foreach(name ${TARGET_NAMES})
  add_executable(${name} ${name}.cc)
endforeach(name ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
        RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}")
