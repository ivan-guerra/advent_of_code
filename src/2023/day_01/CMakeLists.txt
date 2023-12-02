cmake_minimum_required(VERSION 3.13)

project(
  aoc_${AOC_YEAR}_day_1
  DESCRIPTION "AOC 2023 - Day 1 Solutions"
  LANGUAGES CXX)

set(TARGET_NAMES aoc_${AOC_YEAR}_1_1 aoc_${AOC_YEAR}_1_2)

foreach(name ${TARGET_NAMES})
  add_executable(${name} ${name}.cc)
endforeach(name ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
        RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}")
