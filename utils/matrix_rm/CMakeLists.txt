cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

add_executable(matrix_rm test_rm.c)

add_library(rm reference_model/matrix_rm.c)
add_library(theini_handler theini_handler/ini_loader.c)

# target_link_libraries(ini rm)
target_link_libraries(matrix_rm ini)
target_link_libraries(theini_handler rm)
target_link_libraries(matrix_rm theini_handler)