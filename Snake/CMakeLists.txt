cmake_minimum_required(VERSION 3.0)

#Eigen3
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

#OpenCV
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

#Build type
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "set build type")

project(Snake)

add_executable(Snake main.cpp GameInstance.cpp)

#OpenCV libs
target_link_libraries(Snake ${OpenCV_LIBS})