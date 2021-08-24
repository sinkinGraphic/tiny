cmake_minimum_required(VERSION 3.0)

#Resolve cmake error
set(CMAKE_SIZEOF_VOID_P 8)

#GLFW
find_package(glfw3 REQUIRED)

include_directories(${GLFW3_INCLUDE_DIRS})

#GLAD
find_package(glad REQUIRED)
include_directories(${GLAD_INCLUDE_DIRS})

#Build type
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "set build type")

project(TinyGL)

add_executable(TinyGL TinyGL.cpp)

#GLFW libs
target_link_libraries(TinyGL ${GLFW3_LIBS})

#GLAD libs
target_link_libraries(TinyGL ${GLAD_LIBS})
