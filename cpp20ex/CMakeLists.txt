cmake_minimum_required(VERSION 3.20)
set(PROJECT cpp20ex)
project(${PROJECT} LANGUAGES CXX)

#since we're using zip view. maight need cmake upgrade
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#we're using execution policies with clang
find_package(TBB REQUIRED)

add_executable(ex1 ex1.cpp)
target_link_libraries(ex1 PRIVATE TBB::tbb)
target_compile_features(ex1 PRIVATE cxx_std_23)
add_executable(ex2 ex2.cpp)

