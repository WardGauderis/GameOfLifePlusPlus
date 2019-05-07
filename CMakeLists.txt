cmake_minimum_required(VERSION 3.13)
project(GOL)

set(CMAKE_CXX_FLAGS "-O2 -Wextra -Wall -Werror -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual -Wunused -Wduplicated-branches -Wduplicated-cond")
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE HDRS ${GOL_SOURCE_DIR}/src/*.h  )
file(GLOB_RECURSE SRCS ${GOL_SOURCE_DIR}/src/*.cpp)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core    REQUIRED )
find_package(Qt5Widgets REQUIRED )
find_package(Qt5Gui     REQUIRED )

add_executable(GOL ${HDRS} ${SRCS})

qt5_use_modules(GOL Core Widgets Gui)