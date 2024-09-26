set(SRC_HEADER
    # Common
    Common/util.hpp
    # Raycaster
    Raycaster/raycaster.hpp
    # Render
    Render/render.hpp
)

set(SRC_SOURCE
    # Raycaster
    Raycaster/raycaster.cpp
    # Render
    Render/draw.cpp
    Render/events.cpp
    Render/render.cpp
    Render/update.cpp
    Render/window.cpp
    # main
    main.cpp 
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add executable target with source and header files listed in 
# PROJECT_HEADER and PROJECT_SOURCE variable
add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARIES}
)

# Specifies include directories to use when compiling a given target
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
)
