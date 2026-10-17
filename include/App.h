#pragma once
#include <vector>

#define GLFW_INCLUDE_NONE
#include "Particle.h"
#include "Renderer.h"
#include "Window.h"

class App {

private:
    Window window;
    Renderer renderer;
    std::vector<Particle> particles;


    void update();
    void render();

public:

    App();
    void run();
};
