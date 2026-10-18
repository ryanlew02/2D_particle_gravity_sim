#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/gl.h>
#include <Renderer.h>
#include "Particle.h"
#include <vector>
#include <random>

Renderer::Renderer() {

    //get shader sources
    auto [vertexShaderSource, fragmentShaderSource] = Renderer::GetShaderSources();

    //compile shaders and create program
    shaderProgram = Renderer::InitShaderProgram(vertexShaderSource, fragmentShaderSource);

    //get uniform locations
    posLocation = glGetUniformLocation(shaderProgram, "uPosition");
    radiusLocation = glGetUniformLocation(shaderProgram, "uRadius");
    colorLocation = glGetUniformLocation(shaderProgram, "uColor");

    if (posLocation == -1 || radiusLocation == -1) {
        throw std::runtime_error("Could not find position location");
    }

    //tell OpenGL which shader program we want to use
    glUseProgram(shaderProgram);

    const std::vector<float> vertices = CreateCircle(0.0f, 0.0f, 1.0f, 64);

    vertexCount = vertices.size() / 2;

    Renderer::InitVAOVBO(VAO, VBO, vertices.data(), vertexCount);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawCircle(const Particle& particle) const {
    glUseProgram(shaderProgram);

    //send position to the vertex shader
    glUniform2f(posLocation, particle.pos.x, particle.pos.y);
    glUniform1f(radiusLocation, particle.radius);

    glUniform3f(colorLocation, particle.color[0], particle.color[1], particle.color[2]);

    //draw circle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

//create shader program
GLuint Renderer::InitShaderProgram(const char* vertexSource, const char* fragmentSource) {

    //create shader object and get reference
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach vertex shader source to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    //compile vertex shader into machine code
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    //create fragment object and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach fragment shader source to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    //compile fragment shader into machine code
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    //create shader program object and get reference
    GLuint shaderProgram = glCreateProgram();

    //attach both vertex shader and fragment shader to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //link all shaders together in the program
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    //delete the shaders now that they are in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;

}

//create VAO and VBO
void Renderer::InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount) {
    //generate the vao and vbo with 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //make the VAO the current vertex array by binding it
    glBindVertexArray(VAO);

    //bind the VBO specifying array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    //configure the vertex attribute so OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void *>(nullptr));
    //enable the vertex attribute so OpenGL knows how to use it
    glEnableVertexAttribArray(0);

    //bind both the VBO and VAO to 0 so we don't accidentally modify them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//get shader source constants
std::pair<const char*, const char*> Renderer::GetShaderSources() {

    //vertex shader source code
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    uniform vec2 uPosition;
    uniform float uRadius;

    void main()
    {
        vec2 scaledPosition = aPos * uRadius;
        gl_Position = vec4(scaledPosition + uPosition, 0.0, 1.0);
    }
    )";

    //fragment shader source code
    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 uColor;

    void main()
    {
        FragColor = vec4(uColor, 1.0);
    }
    )";

    return {vertexShaderSource, fragmentShaderSource};
}

//create circle with center, radius, and resolution
std::vector<float> Renderer::CreateCircle(const float centerX, const float centerY, const float radius, const int res) {

    std::vector<float> vertices;

    if (res < 3) return vertices;

    vertices.push_back(centerX);
    vertices.push_back(centerY);

    for (int i = 0; i <= res; i++) {

        const float angle = 2.0f * 3.1415926535 * i / res;

        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}