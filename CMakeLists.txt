cmake_minimum_required(VERSION 3.16)

project(Simulation)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)

add_executable(Simulation
    src/main.cpp
)

target_link_libraries(Simulation PRIVATE glfw)