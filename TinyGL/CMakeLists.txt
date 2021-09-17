cmake_minimum_required(VERSION 3.0)

#Resolve cmake error
set(CMAKE_SIZEOF_VOID_P 8)

#Cpp standard
set(CMAKE_CXX_STANDARD 17)

#GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})

#GLAD
find_package(glad REQUIRED)
include_directories(${GLAD_INCLUDE_DIRS})

#Utils
add_subdirectory("Utils")
include_directories("Utils")

project(TinyGL)

add_executable(TinyGL TinyGL.cpp)

#GLFW libs
target_link_libraries(TinyGL ${GLFW3_LIBS})

#GLAD libs
target_link_libraries(TinyGL ${GLAD_LIBS})

#Utils libs
target_link_libraries(TinyGL TinyUtils)

#linux
if(UNIX AND NOT APP)

    #linux libdl
    find_library(DL_LIBRARY libdl.so)
    target_link_libraries(TinyGL ${DL_LIBRARY})

    #linux pthread
    find_library(PTHREAD_LIBRARY libpthread.so)
    target_link_libraries(TinyGL ${PTHREAD_LIBRARY})

    #linux x11
    find_package(X11 REQUIRED)
    target_link_libraries(TinyGL ${X11_LIBRARIES})

endif()