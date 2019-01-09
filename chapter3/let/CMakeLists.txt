cmake_minimum_required(VERSION 3.13)
project(chapter3)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories(../../third-party/GSL/include)
link_directories(/usr/local/lib)

set(SRC_DIR src)
set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/value.cpp
        ${SRC_DIR}/expr.cpp
        ${SRC_DIR}/parser.tab.cc
        ${SRC_DIR}/lex.yy.cpp)

add_executable(chapter3-let ${SOURCES})
target_link_libraries(chapter3-let fmt reflex)