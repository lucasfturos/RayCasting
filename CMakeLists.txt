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

set(PROJECT_HEADER                    # Directory of all header file
  # Common
  src/Common/math.hpp
  src/Common/util.hpp
  # Raycaster
  src/Raycaster/raycaster.hpp
  # Render
  src/Render/render.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # Raycaster
  src/Raycaster/raycaster.cpp
  # Render
  src/Render/draw.cpp
  src/Render/events.cpp
  src/Render/render.cpp
  src/Render/update.cpp
  src/Render/window.cpp
  # main
  src/main.cpp 
)

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

# Add executable target with source and header files listed in 
# PROJECT_HEADER and PROJECT_SOURCE variable
add_executable(${PROJECT_NAME} 
  ${PROJECT_HEADER} 
  ${PROJECT_SOURCE}
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
  PRIVATE ${SDL2_LIBRARIES}
)

# Specifies include directories to use when compiling a given target
target_include_directories(${PROJECT_NAME} 
  PRIVATE src 
  PRIVATE src/Common 
  PRIVATE src/Render
)

# -------------------------------------------------------------------
