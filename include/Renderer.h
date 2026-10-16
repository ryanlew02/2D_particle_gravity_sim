#pragma once

#include <glad/gl.h>
#include <Particle.h>
#include <vector>
#include <random>



//create shader program
GLuint InitShaderProgram(const char* vertexSource, const char* fragmentSource);

//create VAO and VBO
void InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount);

//get shader source constants
std::pair<const char*, const char*> GetShaderSources();

//create circle with center, radius, and resolution
std::vector<float> CreateCircle(const float centerX, const float centerY, const float radius, const int res);

void drawCircle(const Particle& particle, const GLuint& VAO, const GLint& positionLocation, const GLint& radiusLocation, const int vertexCount);