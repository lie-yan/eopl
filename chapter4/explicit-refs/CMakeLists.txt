cmake_minimum_required(VERSION 3.13)
project(chapter4)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(THIRD_PARTY_DIR ../../third-party)
set(GTEST_DIR ${THIRD_PARTY_DIR}/googletest)
include_directories(${THIRD_PARTY_DIR}/GSL/include)
include_directories(${GTEST_DIR}/googletest/include)
include_directories(${GTEST_DIR}/googlemock/include)
link_directories(${GTEST_DIR}/cmake-build-debug/lib)

set(SOURCE_DIR src)

find_package(FLEX)
find_package(BISON)
set(FLEX_EXECUTABLE reflex)
set(FLEX_VERSION reflex-1.0.10)

FLEX_TARGET(Scanner ${SOURCE_DIR}/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp
        COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lex.yy.h)
BISON_TARGET(Parser ${SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${SOURCE_DIR})
set(SOURCES
        ${SOURCE_DIR}/value.cpp
        ${SOURCE_DIR}/expr.cpp
        ${SOURCE_DIR}/env.cpp
        ${SOURCE_DIR}/store.cpp
        ${SOURCE_DIR}/nameless/senv.cpp
#        ${SOURCE_DIR}/nameless/nenv.cpp
#        ${SOURCE_DIR}/nameless/translator.cpp
#        ${SOURCE_DIR}/nameless/interpreter.cpp
        ${SOURCE_DIR}/interpreter.cpp
        ${SOURCE_DIR}/built_in.cpp
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS})

#add_definitions(-DDEBUG)

add_executable(chapter4-explicit-refs
        ${SOURCES}
        ${SOURCE_DIR}/main.cpp)
target_link_libraries(chapter4-explicit-refs fmt reflex)

add_executable(chapter4-explicit-refs-test
        ${SOURCES}
        ${SOURCE_DIR}/explicit_refs_test.cpp)
target_link_libraries(chapter4-explicit-refs-test fmt reflex gtest gtest_main)
