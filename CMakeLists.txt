# ./cmake-build-debug/golc to run the proj
# everything should work out of the box

cmake_minimum_required(VERSION 3.31)
project(golc C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(SDL2 EXCLUDE_FROM_ALL)

add_executable(golc main.c)

if(TARGET SDL2::SDL2main)
    target_link_libraries(golc PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(golc PRIVATE SDL2::SDL2)