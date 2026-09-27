
cmake_minimum_required(VERSION 3.16)

project(ParticleGravitySim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(simulation
    src/main.cpp
    src/gl.c
)

target_include_directories(simulation PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(simulation PRIVATE
    glfw
    OpenGL::GL
)
