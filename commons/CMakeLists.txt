cmake_minimum_required(VERSION 3.21)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# static library by default
if (NOT DEFINED BUILD_SHARD_LIBS)
    set(BUILD_SHARED_LIBS off)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(commons LANGUAGES C VERSION 0.1.0)

include_directories(include)

file(GLOB_RECURSE commons_src src/*.c)
add_library(commons ${commons_src})
target_include_directories(commons PUBLIC include PRIVATE src)
