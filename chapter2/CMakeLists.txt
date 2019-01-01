cmake_minimum_required(VERSION 3.13)
project(chapter2)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(chapter2-value-demo value_demo.cpp value.cpp)

add_executable(chapter2 environment_demo.cpp value.cpp environment.cpp environment.h)
target_link_libraries(chapter2 fmt)