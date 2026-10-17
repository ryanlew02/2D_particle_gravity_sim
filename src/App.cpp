#include "App.h"

#include "Constants.h"
#include "Physics.h"
#include "Renderer.h"

App::App() : window(800, 800, "Physics Simulation") {


    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD\n");
    }

    auto [vertexShaderSource, fragmentShaderSource] = GetShaderSources();

    GLuint shaderProgram = InitShaderProgram(vertexShaderSource, fragmentShaderSource);

    GLint posLocation = glGetUniformLocation(shaderProgram, "uPosition");
    GLint radiusLocation = glGetUniformLocation(shaderProgram, "uRadius");

    if (posLocation == -1) {
        throw std::runtime_error("Could not find position location");
    }

    //tell OpenGL which shader program we want to use
    glUseProgram(shaderProgram);

    const std::vector<float> vertices = CreateCircle(0.0f, 0.0f, 1.0f, 64);

    //create reference containers for the vertex array object and the vertex buffer object
    GLuint VAO, VBO;

    size_t vertexCount = vertices.size() / 2;

    InitVAOVBO(VAO, VBO, vertices.data(), vertexCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> dis2(-3.0f, 3.0f);

    std::vector<Particle> particles;

    for (int i = 0; i < 12; i++) {
        particles.emplace_back(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.05f);
    }
}

void App::run() {

    double lastTime = glfwGetTime();
    double accumulator = 0.0;

    //main loop
    while (!window.shouldClose()) {
        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;

        lastTime = currentTime;

        frameTime = std::min(frameTime, 0.25);
        accumulator += frameTime;

        //physics updates
        while (accumulator >= FIXED_DT) {

            update();
            accumulator -= FIXED_DT;

        }

        render();

        //swap buffers and see if there are any event updates
        glfwSwapBuffers(window);
        glfwPollEvents();
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

}

void App::render() {

    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& particle : particles) {
        drawCircle(particle, VAO, posLocation, radiusLocation, static_cast<int>(vertices.size() / 2));
    }

}