cmake_minimum_required(VERSION 3.20)

project(Gaf VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Gaf STATIC)
add_subdirectory(process)

install(TARGETS Gaf DESTINATION lib)
