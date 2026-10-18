#pragma once
#include <vector>

#define GLFW_INCLUDE_NONE
#include "Particle.h"
#include "Renderer.h"
#include "Window.h"
#include "Vec2.h"

class App {

private:
    Window window;
    Renderer renderer;
    std::vector<Particle> particles;

    bool wasMousePressed = false;
    Vec2 dragStart;


    void update();
    void render();
    void handleInput();

public:

    App();
    void run();
};
