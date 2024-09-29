cmake_minimum_required(VERSION 3.28)

project(
  advent_of_code
  VERSION 1.0
  DESCRIPTION "Advent of Code Solutions")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(set_compile_link_flags)

# Add a new solution target.
function(add_solution year day soln_num)
  if(ARGC LESS 3)
    message(FATAL "add_solution requires three arguments")
  endif()

  set(tgt_name aoc_${year}_${day}_${soln_num})
  project(
    ${tgt_name}
    DESCRIPTION "AOC ${year} - Day ${day} Solution ${soln_num}"
    LANGUAGES CXX)

  add_executable(${tgt_name})
  target_sources(${tgt_name} PRIVATE ${tgt_name}.cc)
  target_link_libraries(${tgt_name} PRIVATE ssl3 crypto)
  install(TARGETS ${tgt_name}
          RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${year}/day_${day})
endfunction()

add_subdirectory(src)
