cmake_minimum_required(VERSION 3.13)
project(chapter3)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories(../../third-party/GSL/include)
link_directories(/usr/local/lib)

set(SOURCE_DIR src)
set(GEN_DIR src/gen)

find_package(FLEX)
find_package(BISON)
set(FLEX_EXECUTABLE reflex)
set(FLEX_VERSION reflex-1.0.10)

FLEX_TARGET(LetScanner ${SOURCE_DIR}/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp
        COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lex.yy.h)
BISON_TARGET(LetParser ${SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
ADD_FLEX_BISON_DEPENDENCY(LetScanner LetParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${SOURCE_DIR})
set(SOURCES
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/value.cpp
        ${SOURCE_DIR}/expr.cpp
        ${FLEX_LetScanner_OUTPUTS}
        ${BISON_LetParser_OUTPUTS})

add_executable(chapter3-let ${SOURCES})
target_link_libraries(chapter3-let fmt reflex)