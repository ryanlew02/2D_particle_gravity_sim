#pragma once
#include <vector>

#include "Particle.h"
#include "Window.h"

class App {

private:
    Window window;
    std::vector<Particle> particles;

    void update();
    void render();

public:

    App();
    void run();
};
