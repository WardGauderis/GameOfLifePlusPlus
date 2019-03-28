cmake_minimum_required(VERSION 3.13)
project(GOL)

set(CMAKE_CXX_FLAGS "-Wextra -Wall")
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE HDRS ${GOL_SOURCE_DIR}/src/*.h  )
file(GLOB_RECURSE SRCS ${GOL_SOURCE_DIR}/src/*.cpp)

add_executable(GOL ${HDRS} ${SRCS})