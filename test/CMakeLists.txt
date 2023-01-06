enable_testing()

add_executable(scanner_test scanner_test.cpp ../src/token.cpp ../src/scanner.cpp ../src/reporter.cpp)

target_link_libraries(scanner_test gtest_main)

include(GoogleTest)
gtest_discover_tests(scanner_test)