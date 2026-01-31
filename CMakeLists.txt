cmake_minimum_required(VERSION 3.10)
project(RoomSimulation3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cari package OpenGL
find_package(OpenGL REQUIRED)

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

# =====================================================================
# PLATFORM DETECTION - OTOMATIS WINDOWS / LINUX
# =====================================================================
if(WIN32)
    message(STATUS "===========================================")
    message(STATUS "  SIMULASI RUANGAN 3D - WINDOWS BUILD")
    message(STATUS "===========================================")
    
    # Include GLFW untuk Windows
    include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include)
    
    if(MSVC)
        # Visual Studio
        link_directories(${CMAKE_SOURCE_DIR}/lib/glfw/lib-vc2022)
        set(GLFW_LIB glfw3)
    else()
        # MinGW
        link_directories(${CMAKE_SOURCE_DIR}/lib/glfw/lib-mingw-w64)
        set(GLFW_LIB libglfw3.a)
    endif()
    
    # Buat executable
    add_executable(${PROJECT_NAME} ${SOURCES})
    
    # Link libraries untuk Windows
    if(MSVC)
        target_link_libraries(${PROJECT_NAME}
            OpenGL::GL
            ${GLFW_LIB}
        )
    else()
        # MinGW membutuhkan library tambahan
        target_link_libraries(${PROJECT_NAME}
            OpenGL::GL
            ${GLFW_LIB}
            gdi32
            user32
            kernel32
        )
    endif()
    
else()
    # LINUX BUILD
    message(STATUS "===========================================")
    message(STATUS "  SIMULASI RUANGAN 3D - LINUX BUILD")
    message(STATUS "===========================================")
    
    # Cari GLFW di sistem Linux
    find_package(glfw3 3.3 REQUIRED)
    
    # Buat executable
    add_executable(${PROJECT_NAME} ${SOURCES})
    
    # Link libraries untuk Linux
    target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        ${CMAKE_DL_LIBS}
    )
endif()

# Copy shaders ke build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)

# Pesan info
message(STATUS "OpenGL Found: ${OPENGL_FOUND}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
