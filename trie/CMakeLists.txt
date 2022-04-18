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

project(trie LANGUAGES C VERSION 0.1.0)

include_directories(include ../commons/include)
add_library(trie src/trie.c)
target_include_directories(trie PUBLIC include PRIVATE src)
