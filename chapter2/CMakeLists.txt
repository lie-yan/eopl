cmake_minimum_required(VERSION 3.13)
project(chapter2)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(chapter2 main.cpp value.cpp)