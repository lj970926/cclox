enable_testing()

file(GLOB_RECURSE LOX_SOURCES ../src/*.cpp)

add_executable(scanner_test scanner_test.cpp ${LOX_SOURCES})
add_executable(ast_printer_test ast_printer_test.cpp ${LOX_SOURCES})
add_executable(parser_test parser_test.cpp ${LOX_SOURCES})
add_executable(executor_test executor_test.cpp ${LOX_SOURCES})

target_link_libraries(scanner_test gtest_main)
target_link_libraries(ast_printer_test gtest_main)
target_link_libraries(parser_test gtest_main)
target_link_libraries(executor_test gtest_main)

include(GoogleTest)
gtest_discover_tests(scanner_test)
gtest_discover_tests(ast_printer_test)
gtest_discover_tests(parser_test)
gtest_discover_tests(executor_test)