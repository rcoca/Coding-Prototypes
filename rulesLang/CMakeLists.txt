project(rules_parser)
cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")
add_definitions("-ftemplate-backtrace-limit=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -pedantic")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Boost 1.71.0 REQUIRED COMPONENTS  system thread)
find_package(Threads REQUIRED)

set(RULES_LANG_LIB_NAME rule_lang)

set(RULES_LANG_SRCS parser.cpp rules_grammar.hpp expression_ast.hpp fusion_expression_ast.hpp expression_ast_print.cpp)
add_library(${RULES_LANG_LIB_NAME} STATIC ${RULES_LANG_SRCS})
SET_TARGET_PROPERTIES(${RULES_LANG_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${RULES_LANG_LIB_NAME} ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
    ${RULES_LANG_LIB_NAME}
    ${Boost_LIBRARIES}
    ${PTHREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT})

