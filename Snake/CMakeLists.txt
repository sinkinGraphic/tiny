cmake_minimum_required(VERSION 3.0)

#Resolve cmake error
set(CMAKE_SIZEOF_VOID_P 8)

#Eigen3
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

#OpenCV
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

#GLFW
find_package(glfw3 REQUIRED)

include_directories(${GLFW3_INCLUDE_DIRS})

#GLAD
find_package(glad REQUIRED)
include_directories(${GLAD_INCLUDE_DIRS})

#Build type
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "set build type")

project(Snake)

add_executable(Snake main.cpp GameInstance.cpp GamePlay.cpp)

#OpenCV libs
target_link_libraries(Snake ${OpenCV_LIBS})

#GLFW libs
target_link_libraries(Snake ${GLFW3_LIBS})

#GLAD libs
target_link_libraries(Snake ${GLAD_LIBS})
