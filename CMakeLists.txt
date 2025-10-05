cmake_minimum_required(VERSION 3.20)

project(Gap VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Gap STATIC)
add_subdirectory(process)
add_subdirectory(net)

install(TARGETS Gap DESTINATION lib)
