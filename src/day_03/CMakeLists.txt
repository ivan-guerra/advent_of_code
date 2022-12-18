cmake_minimum_required(VERSION 3.13...3.22)

project(day_3 DESCRIPTION "Day 3 Solutions"
              LANGUAGES   CXX
)

set(TARGET_NAMES
    aoc_3_1
    aoc_3_2
)

foreach(NAME ${TARGET_NAMES})
    add_executable(${NAME} ${NAME}.cc)
endforeach(NAME ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
    RUNTIME DESTINATION "${AOC_BIN_DIR}/${PROJECT_NAME}"
)
