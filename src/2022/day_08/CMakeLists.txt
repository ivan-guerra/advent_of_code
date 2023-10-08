cmake_minimum_required(VERSION 3.13)

project(aoc_${AOC_YEAR}_day_8 
    DESCRIPTION "AOC 2022 - Day 8 Solutions"
    LANGUAGES   CXX
)

set(TARGET_NAMES
    aoc_2022_8_1
    aoc_2022_8_2
)

foreach(NAME ${TARGET_NAMES})
    add_executable(${NAME} ${NAME}.cc)
endforeach(NAME ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
    RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}"
)
