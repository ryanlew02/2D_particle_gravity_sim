#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <utility>
#include <cstddef>
#include <vector>
#include <algorithm>



GLFWwindow* InitWindow();
GLuint InitShaderProgram(const char* vertexSource, const char* fragmentSource);
void InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount);
std::pair<const char*, const char*> GetShaderSources();
std::vector<float> CreateCircle(float centerX, float centerY, float radius, int res);
void DrawCircle(GLuint& VAO, GLint& positionLocation, float x, float y, int vertexCount);

class Particle {

public:
    float x, y;
    float vx, vy;
    float radius, diameter;
    float restitution = -0.95;


    Particle(float x, float y, float vx, float vy, float radius) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->radius = radius;
        this->diameter = radius * 2;
    }

    void accelerate(float x, float y, float deltaTime) {
        this->vx += x * deltaTime;
        this->vy += y * deltaTime;
    }

    void updatePos(float deltaTime) {
        this->x += vx * deltaTime;
        this->y += vy * deltaTime;
    }

    void drawCircle(GLuint& VAO, GLint& positionLocation, GLint& radiusLocation, int vertexCount) {

        //send position to the vertex shader
        glUniform2f(positionLocation, x, y);
        glUniform1f(radiusLocation, radius);

        //draw circle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    }

    void handleCollisions() {

        float min_boundry = -1.0f + radius;
        float max_boundry = 1.0f - radius;

        if (x < min_boundry) {
            vx *= -0.95;
            x = min_boundry;
        } else if (x > max_boundry) {
            vx *= -0.95;
            x = max_boundry;
        }
        if (y < min_boundry) {
            vy *= -0.95;
            y = min_boundry;
        } else if (y > max_boundry) {
            vy *= -0.95;
            y = max_boundry;
        }

    }

    bool isColliding(const Particle& otherParticle) {
        float dx = x - otherParticle.x;
        float dy = y - otherParticle.y;

        float dist = dx * dx + dy * dy;
        float rDist = radius + otherParticle.radius;

        return dist <= rDist * rDist;

        
    }

};

int main() {

    //init the window and shaderProgram
    GLFWwindow* window = InitWindow();

    //get screenHeight and Width
    int screenHeight, screenWidth;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    if (!window) {
        return -1;
    }

    auto [vertexShaderSource, fragmentShaderSource] = GetShaderSources();
    GLuint shaderProgram = InitShaderProgram(vertexShaderSource, fragmentShaderSource);
    GLint posLocation = glGetUniformLocation(shaderProgram, "uPosition");
    GLint radiusLocation = glGetUniformLocation(shaderProgram, "uRadius");

    if (posLocation == -1) {
        std::cerr << "Could not find position location";
        return -1;
    }

    //tell OpenGL which shader program we want to use
    glUseProgram(shaderProgram);

    std::vector<float> vertices = CreateCircle(0.0f, 0.0f, 1.0f, 64);

    //create reference containers for the vertex array object and the vertex buffer object
    GLuint VAO, VBO;
    InitVAOVBO(VAO, VBO, vertices.data(), vertices.size() / 2);

    float lastTime = static_cast<float>(glfwGetTime());
    float currentTime, deltaTime;
    float gravity = -1.5f;

    std::vector<Particle> particles = {
        Particle(0.3f, 0.75f, 0.0f, 0.0f, 0.2f),
        Particle(-0.3f, 0.35f, 0.0f, 0.0f, 0.1f)
    };

    //main loop
    while (!glfwWindowShouldClose(window)) {

        //calculate delta time and replace current time
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& particle : particles) {
            particle.accelerate(gravity, gravity, deltaTime);
            particle.updatePos(deltaTime);
            particle.handleCollisions();
            particle.drawCircle(VAO, posLocation, radiusLocation, static_cast<int>(vertices.size() / 2));
        }

        for (int i = 0; i < particles.size(); i++) {
            for (int j = i + 1; j < particles.size(); j++) {
                if (particles[i].isColliding(particles[j])) {
                    particles[i].vx *= particles[i].restitution;
                    particles[i].vy *= particles[i].restitution;
                    particles[j].vx *= particles[j].restitution;
                    particles[j].vy *= particles[j].restitution;
                }
            }
        }

        //swap buffers and see if there are any event updates
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //shader destruction
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //main destruction
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

GLFWwindow* InitWindow() {

    //initialize glfw with checks
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    // macOS OpenGL configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Windows OpenGL configuration
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window and make sure it's initialized
    GLFWwindow* window = glfwCreateWindow(
        800, 800, "Physics Simulation", nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    //tells open GL that window is our current context
    glfwMakeContextCurrent(window);

    //enables v-sync
    glfwSwapInterval(0);

    //init version and gladLoadGL
    int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return nullptr;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
    );

    return window;
}

//create shader program
GLuint InitShaderProgram(const char* vertexSource, const char* fragmentSource) {

    //create shader object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach vertex shader source to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    //compile vertex shader into machine code
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    //create fragment object and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach fragment shader source to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    //compile fragment shader into machine code
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    //delete the shaders now that they are in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;

}

//create VAO and VBO
void InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount) {
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //enable the vertex attribute so OpenGL knows how to use it
    glEnableVertexAttribArray(0);
    
    //bind both the VBO and VAO to 0 so we don't accedentally modify them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//get shader source constants
std::pair<const char*, const char*> GetShaderSources() {

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

    void main()
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )";

    return {vertexShaderSource, fragmentShaderSource};   
}

//create circle with center, radius, and resolution
std::vector<float> CreateCircle(float centerX, float centerY, float radius, int res) {

    std::vector<float> vertices;

    if (res < 3) return vertices;

    vertices.push_back(centerX);
    vertices.push_back(centerY);

    for (int i = 0; i <= res; i++) {
        
        float angle = 2.0f * 3.1415926535 * i / res;

        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}
