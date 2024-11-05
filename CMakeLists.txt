cmake_minimum_required(VERSION 3.10)
project(OpenGLTriangle)

# Add source files
set(SOURCES
    src/helloTriangle.cpp
    src/glad.c
)

# Add include directories
include_directories(include)

# Find OpenGL
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)

# Platform-specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(APPLE)
    find_library(COCOA Cocoa REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${COCOA})
endif()
