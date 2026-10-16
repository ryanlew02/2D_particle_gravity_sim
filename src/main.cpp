#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Particle.h"
#include "Physics.h"
#include "Vec2.h"
#include "Constants.h"
#include "Renderer.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// ****** MAIN FUNCTION ****** //

int main() {

    //init the window and shaderProgram
    GLFWwindow* window = InitWindow();

    //make sure window is created
    if (!window) {
        return -1;
    }

    //get screenHeight and Width --> unused
    // int screenHeight, screenWidth;
    // glfwGetWindowSize(window, &screenWidth, &screenHeight);

    auto [vertexShaderSource, fragmentShaderSource] = GetShaderSources();
    GLuint shaderProgram = InitShaderProgram(vertexShaderSource, fragmentShaderSource);
    GLint posLocation = glGetUniformLocation(shaderProgram, "uPosition");
    GLint radiusLocation = glGetUniformLocation(shaderProgram, "uRadius");

    if (posLocation == -1) {
        std::cerr << "Could not find position location";
        return -1;
    }

    //tell OpenGL which shader program we want to use
    glUseProgram(shaderProgram);

    const std::vector<float> vertices = CreateCircle(0.0f, 0.0f, 1.0f, 64);

    //create reference containers for the vertex array object and the vertex buffer object
    GLuint VAO, VBO;
    InitVAOVBO(VAO, VBO, vertices.data(), vertices.size() / 2);

    double accumulator = 0.0;
    double lastTime = glfwGetTime();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> dis2(-3.0f, 3.0f);

    std::vector<Particle> particles;

    for (int i = 0; i < 12; i++) {
        particles.emplace_back(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.05f);
    }

    //main loop
    while (!glfwWindowShouldClose(window)) {

        const double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

        frameTime = std::min(frameTime, 0.25);
        accumulator += frameTime;

        //physics updates
        while (accumulator >= FIXED_DT) {

            //apply gravity
            for (size_t i = 0; i < particles.size(); i++) {
                for (size_t j = i + 1; j < particles.size(); j++) {
                    applyGravity(particles[i], particles[j]);
                }
            }

            //update positions
            for (auto& particle : particles) {
                particle.updatePos();
            }

            //handle particle collisions
            for (size_t i = 0; i < particles.size(); i++) {
                for (size_t j = i + 1; j < particles.size(); j++) {
                    if (particles[i].isColliding(particles[j])) {
                        handleCollision(particles[i], particles[j]);
                    }
                }
            }

            for (auto& particle : particles) {
                particle.handleCollisions();
            }

            accumulator -= FIXED_DT;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& particle : particles) {
                drawCircle(particle, VAO, posLocation, radiusLocation, static_cast<int>(vertices.size() / 2));
        }

        //swap buffers and see if there are any event updates
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //shader destruction
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //main destruction
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}