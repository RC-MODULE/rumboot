cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

add_executable(bin_generator bin_generator.c)

add_library(theini_gen_handler theini_handler/ini_gen_loader.c)

target_link_libraries(bin_generator ini)
target_link_libraries(bin_generator theini_gen_handler)