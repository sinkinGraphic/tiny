cmake_minimum_required(VERSION 3.0)

project(TinyUtils)

#Settings and Definitions
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(TinyUtils STATIC FileHelper.cpp Paths.cpp)
