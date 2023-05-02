add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
    -Wno-sign-compare
    -std=c++2a
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

#add_subdirectory(2015)
#add_subdirectory(2016)
#add_subdirectory(2017)
add_subdirectory(2018)
#add_subdirectory(2022)
