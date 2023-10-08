cmake_minimum_required(VERSION 3.13)

project(
  aoc_${AOC_YEAR}_day_4
  DESCRIPTION "AOC ${AOC_YEAR} - Day 4 Solutions"
  LANGUAGES CXX)

set(TARGET_NAMES aoc_${AOC_YEAR}_4_1 aoc_${AOC_YEAR}_4_2)

foreach(name ${TARGET_NAMES})
  add_executable(${name} ${name}.cc)

  target_link_libraries(${name} PRIVATE ssl3 crypto)
endforeach(name ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
        RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}")
