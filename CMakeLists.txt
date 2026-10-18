
cmake_minimum_required(VERSION 3.16)

project(ParticleGravitySim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)
find_package(OpenGL REQUIRED)

add_executable(simulation
    src/main.cpp
    src/gl.c
        include/Vec2.h
        include/Particle.h
        src/Particle.cpp
        src/Physics.cpp
        include/Physics.h
        include/Constants.h
        src/Renderer.cpp
        include/Renderer.h
        include/Window.h
        src/Window.cpp
        src/App.cpp
        include/App.h
)

target_include_directories(simulation PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(simulation PRIVATE
    glfw
    OpenGL::GL
)
