cmake_minimum_required(VERSION 3.5)
set(PROJECT kvFilesystemBackedDB)
project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(${CMAKE_SOURCE_DIR}/cpp-httplib)

add_executable(${PROJECT} main.cpp)
target_link_libraries(${PROJECT}  ${Boost_LIBRARIES})
