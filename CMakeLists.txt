
cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

project(AdventOfCode2022 VERSION 1.0 LANGUAGES C)

add_executable(advent2022 advent2022.c src/days.c)
target_include_directories( advent2022 PRIVATE "${PROJECT_SOURCE_DIR}/include")  