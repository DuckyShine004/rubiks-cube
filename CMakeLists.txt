cmake_minimum_required(VERSION 3.22.1)
project(rubiks-cube)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(include)

# Add source files
set(SOURCE_FILES src/main.cpp src/glad.c)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set up OpenGL preference so you don't get the annoying warnings each time you run cmake
set(OpenGL_GL_PREFERENCE "GLVND")

# Find and link OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARIES} ${CMAKE_DL_LIBRARIES} sfml-audio)
