#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <utility>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <random>

// ****** FUNCTION & CLASS DECLARATIONS ****** //

class Particle;
struct Vec2;
GLFWwindow* InitWindow();
GLuint InitShaderProgram(const char* vertexSource, const char* fragmentSource);
void InitVAOVBO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t vertexCount);
std::pair<const char*, const char*> GetShaderSources();
std::vector<float> CreateCircle(float centerX, float centerY, float radius, int res);
void drawCircle(const Particle& particle, const GLuint& VAO, const GLint& positionLocation, const GLint& radiusLocation, const int vertexCount);
void handleCollision(Particle& a, Particle& b);
void applyGravity(Particle& a, Particle& b);
constexpr float FIXED_DT = 1.0f / 120.0f;

// ****** CLASS & STRUCt IMPLEMENTATIONS ****** //

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;

    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vec2 operator+(const Vec2& other) const {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2& other) const {
        return {x - other.x, y - other.y};
    }

    Vec2 operator*(float scaler) const {
        return {x * scaler, y * scaler};
    }

    Vec2 operator/(float scaler) const {
        return {x / scaler, y / scaler};
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(const float scaler) {
        x *= scaler;
        y *= scaler;
        return *this;
    }

    Vec2& operator/=(const float scaler) {
        x /= scaler;
        y /= scaler;
        return *this;
    }

    Vec2 operator-() const {
        return {-x, -y};
    }

    [[nodiscard]] float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] Vec2 normalized() const {
        float len = length();

        if (len < 0.0001f) {
            return {0.0f, 0.0f};
        }

        return *this / len;
    }
};

class Particle {

public:
    Vec2 pos;
    Vec2 v;
    float radius;
    float restitution = 0.95f;
    float density, mass;


    Particle(const float x, const float y, float vx, float vy, float radius, float density = 1.0f)
    : pos(x, y), v(x, y), radius(radius), density(density), mass(density * 3.14159265 * radius * radius) {}

    void accelerate(const Vec2& acc) {
        v += acc * FIXED_DT;
    }

    void updatePos() {
        pos += v * FIXED_DT;
    }

    void handleCollisions() {

        float min_boundry = -1.0f + radius;
        float max_boundry = 1.0f - radius;

        if (pos.x < min_boundry) {
            pos.x = min_boundry;

            if (v.x < 0.0f) {
                v.x  *= -restitution;
            }
        } else if (pos.x > max_boundry) {
            pos.x = max_boundry;

            if (v.x  > 0.0f) {
                v.x  *= -restitution;
            }
        }
        if (pos.y < min_boundry) {
            pos.y = min_boundry;

            if (v.y < 0.0f) {
                v.y *= -restitution;
            }
        } else if (pos.y > max_boundry) {
            pos.y = max_boundry;

            if (v.y > 0.0f) {
                v.y *= -restitution;
            }
        }
    }

    //checks if 2 circles are colliding
    [[nodiscard]] bool isColliding(const Particle& otherParticle) const {
        Vec2 diff = pos - otherParticle.pos;
        float rDist = radius + otherParticle.radius;

        return diff.lengthSquared() <= rDist * rDist;
    }

    //apply's a force to current object
    void applyForce(const Vec2& force) {

        Vec2 acceleration = force / mass;
        v += acceleration * FIXED_DT;
        
    }
};

// ****** MAIN FUNCTION ****** //

int main() {

    //init the window and shaderProgram
    GLFWwindow* window = InitWindow();

    //make sure window is created
    if (!window) {
        return -1;
    }

    //get screenHeight and Width --> unused
    // int screenHeight, screenWidth;
    // glfwGetWindowSize(window, &screenWidth, &screenHeight);

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

    const std::vector<float> vertices = CreateCircle(0.0f, 0.0f, 1.0f, 64);

    //create reference containers for the vertex array object and the vertex buffer object
    GLuint VAO, VBO;
    InitVAOVBO(VAO, VBO, vertices.data(), vertices.size() / 2);

    double accumulator = 0.0;
    double lastTime = glfwGetTime();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> dis2(1.0f, 5.0f);

    std::vector<Particle> particles = {
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f),
        Particle(dis(gen), dis(gen), dis2(gen), dis2(gen), 0.06f)

    };

    //main loop
    while (!glfwWindowShouldClose(window)) {

        const double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

        frameTime = std::min(frameTime, 0.25);
        accumulator += frameTime;

        //physics updates
        while (accumulator >= FIXED_DT) {

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

            accumulator -= FIXED_DT;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& particle : particles) {
                drawCircle(particle, VAO, posLocation, radiusLocation, static_cast<int>(vertices.size() / 2));
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

// ****** FUNCTION IMPLEMENTATIONS ****** //

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
        glfwDestroyWindow(window);
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void *>(nullptr));
    //enable the vertex attribute so OpenGL knows how to use it
    glEnableVertexAttribArray(0);
    
    //bind both the VBO and VAO to 0 so we don't accidentally modify them
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
std::vector<float> CreateCircle(const float centerX, const float centerY, const float radius, const int res) {

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

void drawCircle(const Particle& particle, const GLuint& VAO, const GLint& positionLocation, const GLint& radiusLocation, const int vertexCount) {

    //send position to the vertex shader
    glUniform2f(positionLocation, particle.pos.x, particle.pos.y);
    glUniform1f(radiusLocation, particle.radius);

    //draw circle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

void handleCollision(Particle& a, Particle& b) {
    const Vec2 d = b.pos - a .pos;

    float dist = d.length();
    const float rSum = a.radius + b.radius;

    if (dist >= rSum) {
        return;
    }

    Vec2 normal;

    if (dist > 0.00001f) {
        normal = d / dist;
    } else {
        normal = {1.0f, 0.0f};
        dist = 0.0f;
    }

    Vec2 rv = b.v - a.v;

    const float invMassA = 1.0f / a.mass;
    const float invMassB = 1.0f / b.mass;

    const float velocityAlongNormal = rv.dot(normal);

    const float e = std::min(a.restitution, b.restitution);

    if (velocityAlongNormal < 0.0f) {
        const float impulse = -(1.0f + e)
        * velocityAlongNormal 
        / (invMassA + invMassB);

        Vec2 impulseVec = normal * impulse;

        a.v -= impulseVec * invMassA;
        b.v += impulseVec * invMassB;
    }

    float pen = rSum - dist;

    if (pen > 0.0f) {
        const float correction = pen / (invMassA + invMassB);

        a.pos -= normal * invMassA * correction;
        b.pos += normal * invMassB * correction;
    }

}

void applyGravity(Particle& a, Particle& b) {
    constexpr float G = 5.0f;
    constexpr float SOFTENING = 0.02f;

    const Vec2 direction = b.pos - a.pos;

    const float distSq = direction.lengthSquared() + SOFTENING * SOFTENING;

    const float invDist = 1.0f / std::sqrt(distSq);

    const float forceMagnitude = G * a.mass * b.mass * invDist * invDist * invDist;

    const Vec2 force = direction * forceMagnitude;

    a.applyForce(force);
    b.applyForce(-force);
}