#pragma once

#define GLFW_INCLUDE_NONE
#include <string>
#include <GLFW/glfw3.h>

#include "Vec2.h"

class Window {

private:
    GLFWwindow* window = nullptr;

public:
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;

    GLFWwindow* getWindow() const;
    Vec2 getMousePosition() const;
    bool isMousePressed(int button) const;
    void setTitle(const std::string& title) const;

};
