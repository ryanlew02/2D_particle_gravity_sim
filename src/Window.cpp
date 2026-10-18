
#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

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
