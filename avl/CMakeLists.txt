if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# static library by default
if (NOT DEFINED BUILD_SHARD_LIBS)
    set(BUILD_SHARED_LIBS off)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(avl LANGUAGES C CXX VERSION 0.1.0)

include_directories(include)
add_library(avl src/avl.c src/dbg.c)
target_include_directories(avl PUBLIC include PRIVATE src)

## tests
add_subdirectory(test/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(main_test test/main_test.cc)
target_link_libraries(main_test gtest_main avl)
