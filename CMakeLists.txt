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

# libs
set(libs avl)
foreach(lib IN LISTS libs)
    add_subdirectory(${lib})
endforeach()

## gtest
add_subdirectory(tests/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include(GoogleTest)

# tests
#add_executable(avl_tests tests/avl_tests.cc)
#target_link_libraries(avl_tests gtest_main avl)
file(GLOB tests_sources CONFIGURE_DEPENDS tests/*.cc)
add_executable(main_tests ${tests_sources})
target_link_libraries(main_tests gtest_main ${libs})
target_include_directories(main_tests PRIVATE tests)
gtest_discover_tests(main_tests)
