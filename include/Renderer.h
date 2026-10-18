#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <Particle.h>
#include <vector>
#include <random>

class Renderer {

private:

    GLuint VAO = 0, VBO = 0;
    GLuint shaderProgram = 0;

    GLint posLocation = -1;
    GLint radiusLocation = -1;

    int vertexCount = 0;


public:

    Renderer();
    ~Renderer();

    void drawCircle(const Particle& particle) const;
    void clear();

    //create shader program
    static GLuint InitShaderProgram(const char* vertexSource, const char* fragmentSource);

    //create VAO and VBO
    static void InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount);

    //get shader source constants
    static std::pair<const char*, const char*> GetShaderSources();

    //create circle with center, radius, and resolution
    static std::vector<float> CreateCircle(const float centerX, const float centerY, const float radius, const int res);
};