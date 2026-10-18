#include "App.h"

#define GLFW_INCLUDE_NONE
#include <stdexcept>
#include <random>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "Constants.h"
#include "Physics.h"
#include "Renderer.h"

App::App() : window(800, 800, "Physics Simulation") {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> dis2(-2.0f, 2.0f);
    std::uniform_real_distribution<float> color(0.0f, 1.0f);


    //main cool option
    particles.emplace_back(0.0f, 0.0f, 0.0f, 0.0f, 0.10f, std::array<float, 3>{1.0f, 0.83f, 0.30f}, 100.0f);
    particles.emplace_back(-0.5f, 0.5f, 0.0f, -1.5f, 0.05f, std::array<float, 3>{color(gen), color(gen), color(gen)});
    particles.emplace_back(0.5f, -0.5f, 0.0f, 1.5f, 0.05f, std::array<float, 3>{color(gen), color(gen), color(gen)});
    particles.emplace_back(0.5f, 0.5f, 1.5f, 0.0f, 0.05f, std::array<float, 3>{color(gen), color(gen), color(gen)});
    particles.emplace_back(-0.5f, -0.5f, -1.5f, 0.0f, 0.05f, std::array<float, 3>{color(gen), color(gen), color(gen)});


    //other option
    // for (int i = 0; i < 10; i++) {
    //     particles.emplace_back(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.05f, std::array<float, 3>{color(gen), color(gen), color(gen)});
    // }
    // particles.emplace_back(0.0f, 0.0f, 0.0f, 0.0f, 0.10f, std::array<float, 3>{1.0f, 0.83f, 0.30f}, 100.0f);


}

void App::run() {

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    double fpsTimer = glfwGetTime();
    int frameCount = 0;
    double lastTime = glfwGetTime();
    double accumulator = 0.0;

    //main loop
    while (!window.shouldClose()) {

        frameCount++;

        const double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        double elapsed = currentTime - fpsTimer;

        if (elapsed >= 1.0) {
            double fps = frameCount / elapsed;

            window.setTitle("Physics Simulation - FPS: " + std::to_string(static_cast<int>(fps)));

            frameCount = 0;
            fpsTimer = currentTime;
        }

        lastTime = currentTime;

        frameTime = std::min(frameTime, 0.25);
        accumulator += frameTime;

        //physics updates
        while (accumulator >= FIXED_DT) {

            update();
            accumulator -= FIXED_DT;

        }

        handleInput();

        render();

        //swap buffers and see if there are any event updates
        window.swapBuffers();
        window.pollEvents();
    }




}



void App::update() {

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

    //handle sun collisions
    for (size_t i = 1; i < particles.size();) {
        if (particles[0].isColliding(particles[i])) {
            particles.erase(particles.begin() + i);
        } else {
            i++;
        }
    }

    //handle particle collisions
    for (size_t i = 1; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            if (particles[i].isColliding(particles[j])) {
                handleCollision(particles[i], particles[j]);
            }
        }
    }

    // for (auto& particle : particles) {
    //     particle.handleCollisions();
    // }

}

void App::render() {

    renderer.clear();

    for (auto& particle : particles) {
        renderer.drawCircle(particle);
    }

}

void App::handleInput() {

    bool pressed = window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT);

    if (pressed && !wasMousePressed) {
        dragStart = window.getMousePosition();
    }

    if (!pressed && wasMousePressed) {

        Vec2 dragEnd = window.getMousePosition();
        Vec2 velocity = (dragEnd - dragStart) * 3.0f;


        particles.emplace_back(
            dragStart.x,
            dragStart.y,
            velocity.x,
            velocity.y,
            0.05,
            std::array<float, 3>{0.3f, 0.8f, 1.0f}
        );
    }
    wasMousePressed = pressed;
}
