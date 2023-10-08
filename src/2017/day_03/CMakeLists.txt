cmake_minimum_required(VERSION 3.13)

project(
  aoc_${AOC_YEAR}_day_3
  DESCRIPTION "AOC ${AOC_YEAR} - Day 3 Solutions"
  LANGUAGES CXX)

set(TARGET_NAMES aoc_${AOC_YEAR}_3_1)

foreach(name ${TARGET_NAMES})
  add_executable(${name} ${name}.cc)
endforeach(name ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
        RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}")
