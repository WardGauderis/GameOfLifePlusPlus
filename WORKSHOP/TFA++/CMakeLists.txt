cmake_minimum_required(VERSION 3.14)
project(TEST)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TFA
        DFA.cpp
        DFA.h
        main.cpp)
