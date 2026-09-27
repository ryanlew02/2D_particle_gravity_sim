cmake_minimum_required(VERSION 3.20)

project(ParticleGravitySim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(simulation
    src/main.cpp
    src/Application.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Simulation.cpp
    src/Window.cpp
)

target_include_directories(simulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(simulation PRIVATE glfw OpenGL::GL)

if(APPLE)
    # Apple marks OpenGL as deprecated; silence the warnings.
    target_compile_definitions(simulation PRIVATE GL_SILENCE_DEPRECATION)
endif()
