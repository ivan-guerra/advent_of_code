cmake_minimum_required(VERSION 3.13)

project(aoc_${AOC_YEAR}_day_23
    DESCRIPTION "AOC ${AOC_YEAR} - Day 23 Solutions"
    LANGUAGES   CXX
)

set(TARGET_NAMES
    aoc_${AOC_YEAR}_23_1
    aoc_${AOC_YEAR}_23_2
)

foreach(NAME ${TARGET_NAMES})
    add_executable(${NAME} ${NAME}.cc)
endforeach(NAME ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
    RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}"
)
