
#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glad/gl.h"


Window::Window(int width, int height, const char* title) {

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    // macOS OpenGL configuration
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Windows OpenGL configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        const char* description;
        int error = glfwGetError(&description);
        std::cerr << "GLFW Error: " << error << ": " << (description ? description : "Unknown") << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    glViewport(0, 0 , width, height);
}

Window::~Window() {

    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

GLFWwindow* Window::getWindow() const {
    return window;
}

Vec2 Window::getMousePosition() const {

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (width == 0 || height == 0) {
        return Vec2(0.0f, 0.0f);
    }

    return {
        static_cast<float>(2.0 * x / width - 1.0),
        static_cast<float>(1.0 - 2.0 * y / height)
    };

}

bool Window::isMousePressed(int button) const {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

void Window::setTitle(const std::string& title) const {
    glfwSetWindowTitle(window, title.c_str());
}






