cmake_minimum_required(VERSION 3.5)
set(PROJECT groupAnagrams)
project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
find_package(Threads REQUIRED)

add_executable(${PROJECT} main.cpp)
target_link_libraries(${PROJECT} GTest::GTest GTest::Main ${CMAKE_THREAD_LIBS_INIT})
add_test(
    NAME ${PROJECT}
    COMMAND ${PROJECT}
)