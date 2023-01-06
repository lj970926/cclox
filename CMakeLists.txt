cmake_minimum_required(VERSION 3.23)
project(cclox)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE LOX_SOURCES src/*.cpp)
include_directories(include third_party/googletest/include)
add_executable(${PROJECT_NAME} ${LOX_SOURCES})
add_subdirectory(third_party/googletest)
add_subdirectory(test)
