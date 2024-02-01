cmake_minimum_required(VERSION 2.8)
project(parseCmdlineExecWhitelisted)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

find_package(Boost REQUIRED COMPONENTS regex thread filesystem system)
message(STATUS "Boost version: ${Boost_VERSION}")


SET(FSM_PARSER "fsm_parser.cpp")
add_executable(fsmParser ${FSM_PARSER})
target_link_libraries(fsmParser     ${Boost_LIBRARIES})
