cmake_minimum_required(VERSION 3.10)
project(RoomSimulation3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cari package OpenGL
find_package(OpenGL REQUIRED)

# Cari GLFW
find_package(glfw3 3.3 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/lib/glm
)

# Source files
set(SOURCES
    src/main.cpp
    lib/glad/src/glad.c
)

# Buat executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${CMAKE_DL_LIBS}
)

# Copy shaders ke build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Pesan info
message(STATUS "===========================================")
message(STATUS "  SIMULASI RUANGAN 3D - GRAFIKA KOMPUTER")
message(STATUS "===========================================")
message(STATUS "OpenGL Found: ${OPENGL_FOUND}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
