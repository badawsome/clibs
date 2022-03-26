include_directories(include)
add_library(avl STATIC src/avl.c src/dbg.c)
target_include_directories(avl PUBLIC include PRIVATE src)

## tests
add_subdirectory(test/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(main_test test/main_test.cc)
target_link_libraries(main_test gtest_main avl)
