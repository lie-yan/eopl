cmake_minimum_required(VERSION 3.13)
project(chapter2)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(chapter2-value-demo value_demo.cpp value.cpp)

add_executable(chapter2 main.cpp value.cpp environment.cpp environment.h)

