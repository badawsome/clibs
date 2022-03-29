cmake_minimum_required(VERSION 3.21)
project(clibs C CXX)

set(CMAKE_C_STANDARD 17)
add_definitions(-DCMAKE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DCLIBS_DEBUG)
endif()

add_subdirectory(avl)
