cmake_minimum_required(VERSION 3.10)

# set project name
project(GL2 VERSION 0.0.1 LANGUAGES CXX)

# c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../common)

add_executable(gl2 main.cpp)

