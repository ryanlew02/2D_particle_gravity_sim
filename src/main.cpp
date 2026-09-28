#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

//vertex shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
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

int main() {

    //initialize glfw with checks
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // macOS OpenGL configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    //triangle vertices
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //bottom left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //bottom right corner
        0.0f, 0.5f * float(sqrt(3)) / 3, 0.0f //upper point
    };

    //create window and make sure it's initialized
    GLFWwindow* window = glfwCreateWindow(
        800, 800, "Physics Simulation", nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //init version and gladLoadGL
    int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    //sets the background color to blue
    glViewport(0, 0, 800, 800);

    //create shader object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //attach vertex shader source to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //compile vertex shader into machine code
    glCompileShader(vertexShader);

    //create fragment object and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //attach fragment shader source to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //compile fragment shader into machine code
    glCompileShader(fragmentShader);

    //create shader program object and get reference
    GLuint shaderProgram = glCreateProgram();

    //attach both vertex shader and fragment shader to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //link all shaders together in the program
    glLinkProgram(shaderProgram);

    //delete the shaders now that they are in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //create reference containers for the vertex array object and the vertex buffer object
    GLuint VAO, VBO;

    //generate the vao and vbo with 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //make the VAO the current vertex array by binding it
    glBindVertexArray(VAO);

    //bind the VBO specifying array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //configure the vertex attribute so OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //enable the vertex attribute so OpenGL knows how to use it
    glEnableVertexAttribArray(0);
    
    //bind both the VBO and VAO to 0 so we don't accedentally modify them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //main loop
    while (!glfwWindowShouldClose(window)) {
        //background color
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //tell OpenGL which shader program we want to use
        glUseProgram(shaderProgram);
        //bind the VBO so OpenGL knows how to use it
        glBindVertexArray(VBO);
        //draw the triangles using the GL_TRIANGLES primitive
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);



        glfwPollEvents();
    }


    //shader destruction
    glDeleteVertexArrays(1, &VBO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //main destruction
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}