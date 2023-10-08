cmake_minimum_required(VERSION 3.13)

project(
  aoc_${AOC_YEAR}_day_5
  DESCRIPTION "AOC 2022 - Day 5 Solutions"
  LANGUAGES CXX)

set(TARGET_NAMES aoc_2022_5_1 aoc_2022_5_2)

foreach(name ${TARGET_NAMES})
  add_executable(${name} ${name}.cc)
endforeach(name ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
        RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}")
