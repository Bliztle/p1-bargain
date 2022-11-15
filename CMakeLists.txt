cmake_minimum_required(VERSION 3.1)
project(p1_bargain C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

include_directories("${PROJECT_SOURCE_DIR}/fetch")
add_subdirectory(fetch)
# add_subdirectory(libraries)
# include_directories(./libraries/curl-t-2)