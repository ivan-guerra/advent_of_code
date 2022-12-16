add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
    -std=c++17
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_subdirectory(day_1)
add_subdirectory(day_2)
add_subdirectory(day_3)
add_subdirectory(day_4)
add_subdirectory(day_5)
