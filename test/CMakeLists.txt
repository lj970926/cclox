enable_testing()

file(GLOB_RECURSE LOX_SOURCES ../src/*.cpp)

add_executable(scanner_test scanner_test.cpp ${LOX_SOURCES})

target_link_libraries(scanner_test gtest_main)

include(GoogleTest)
gtest_discover_tests(scanner_test)