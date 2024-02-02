cmake_minimum_required(VERSION 3.5)
set(PROJECT russianDollEnvelopes)
project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT} main.cpp)

