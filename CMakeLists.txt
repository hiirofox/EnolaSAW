cmake_minimum_required(VERSION 3.10)
project(EnolaSAW)

#set(CMAKE_CXX_FLAGS_DEBUG "-Og /MD")
set(CMAKE_CXX_FLAGS_RELEASE "-OFast /MD")

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${WINDOWS_SDK_LIB_PATH})
add_executable(EnolaSAW 
    src/main.cpp 
    src/waveout.cpp
    src/dbg.cpp
)

add_subdirectory(external/glfw-3.4)
target_link_libraries(EnolaSAW glfw)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    target_link_libraries(EnolaSAW glfw ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL not found!")
endif()