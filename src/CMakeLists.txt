add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
    -Wno-sign-compare
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_subdirectory(2022)
