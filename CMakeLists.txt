project(EnolaSAW)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(EnolaSAW src/main.cpp)

add_subdirectory(external/glfw-3.4)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw-3.4/include)
target_link_libraries(EnolaSAW glfw)

find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
    target_link_libraries(EnolaSAW glfw ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL not found!")
endif()