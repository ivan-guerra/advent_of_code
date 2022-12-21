cmake_minimum_required(VERSION 3.13...3.22)

project(day_12 DESCRIPTION "Day 12 Solutions"
               LANGUAGES   CXX
)

set(TARGET_NAMES
    aoc_12_1
    aoc_12_2
)

foreach(NAME ${TARGET_NAMES})
    add_executable(${NAME} ${NAME}.cc)
endforeach(NAME ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
    RUNTIME DESTINATION "${AOC_BIN_DIR}/${PROJECT_NAME}"
)
