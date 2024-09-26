# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)  # CMake version check

# --- Project settings ----------------------------------------------

set(PROJECT_NAME                      # Define all project info
    Demo 
    LANGUAGES CXX
    VERSION 1.0.0
)

project(${PROJECT_NAME})              # Create project 

set(CMAKE_CXX_STANDARD 20)            # Enable c++20 standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# List the image files
set(RESOURCES_DIRECTORY 
    "${CMAKE_SOURCE_DIR}/img"
)

# Copy the entire img directory to the binary directory during build
file(COPY ${RESOURCES_DIRECTORY} 
    DESTINATION ${CMAKE_BINARY_DIR}
)

# Flags of compiling
add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g -O2
)

add_subdirectory(src)

# -------------------------------------------------------------------
