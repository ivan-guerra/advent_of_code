cmake_minimum_required(VERSION 3.13...3.22)

project(aoc_${AOC_YEAR}_day_17
    DESCRIPTION "AOC ${AOC_YEAR} - Day 17 Solutions"
    LANGUAGES   CXX
)

set(TARGET_NAMES
    aoc_${AOC_YEAR}_17_1
    aoc_${AOC_YEAR}_17_2
)

foreach(NAME ${TARGET_NAMES})
    add_executable(${NAME} ${NAME}.cc)

    target_link_libraries(${NAME}
        PRIVATE ssl3
                crypto
    )
endforeach(NAME ${TARGET_NAMES})

install(TARGETS ${TARGET_NAMES}
    RUNTIME DESTINATION "${AOC_BIN_DIR}/${AOC_YEAR}/${PROJECT_NAME}"
)
