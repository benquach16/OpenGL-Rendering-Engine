set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_FLAGS "-g -Werror -Wold-style-cast -Wpedantic" )

cmake_minimum_required (VERSION 2.8.11)
project (OpenGLRenderer)
set(SOURCES
    ./src/main.cpp
    )
set(DIRS
    ./src
    )
add_executable(OpenGLRenderer ${SOURCES})
include_directories(${DIRS})    
