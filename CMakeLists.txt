cmake_minimum_required(VERSION 3.31)
project(golc C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(golc main.c)
target_link_libraries(golc ${SDL2_LIBRARIES})
