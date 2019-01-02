cmake_minimum_required(VERSION 3.13)
project(chapter2)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories(../third-party/GSL/include)
link_directories(/usr/local/lib)

add_executable(chapter2-value-demo value_demo.cpp value.cpp)

add_executable(chapter2-env-demo env_demo.cpp value.cpp)
target_link_libraries(chapter2-env-demo fmt)