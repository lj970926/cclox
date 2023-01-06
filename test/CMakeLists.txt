enable_testing()

add_executable(interpreter_test interpreter_test.cpp)

target_link_libraries(interpreter_test gtest)

include(GoogleTest)
gtest_discover_tests(interpreter_test)