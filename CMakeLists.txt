cmake_minimum_required(VERSION 3.21)
project(clibs C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DCMAKE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DCLIBS_DEBUG)
endif()

# libs
set(libs avl commons)
foreach(lib IN LISTS libs)
    add_subdirectory(${lib})
endforeach()

## gtest
add_subdirectory(tests/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} tests/include)
include(GoogleTest)

# tests
file(GLOB tests_sources CONFIGURE_DEPENDS tests/*.cc)
add_executable(main_test ${tests_sources})
target_link_libraries(main_test gtest_main ${libs})
target_include_directories(main_test PRIVATE tests)
gtest_discover_tests(main_test)

## TODO benchmark
#include(FetchContent)
#FetchContent_Declare(googlebenchmark
#        GIT_REPOSITORY https://github.com/google/benchmark
#        )
#FetchContent_MakeAvailable(googlebenchmark)
#file(GLOB bench_sources CONFIGURE_DEPENDS benchmark/*.cc)
#add_executable(main_bench ${bench_sources})
#target_link_libraries(main_bench benchmark::benchmark ${libs})
