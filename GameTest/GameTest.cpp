#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#pragma comment(lib, "opengl32")

void SetupViewport(GLFWwindow* window) {
    int width;
    int height;

    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    float aspect = (float)width / height;

    glLoadIdentity();
    glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, 1.0, -1.0);
}

void OnFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    SetupViewport(window);
}

GLFWwindow* CreateWindow() {
    if (!glfwInit()) {
        return nullptr;
    }
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeCallback);
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error initializing GLEW";
    }
    return window;
}

const auto vertexShader = R"(
    #version 330 core
    layout(location = 0) in vec3 vPosition;
    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(vPosition, 1.0);
    }
)";

const auto fragmentShader = R"(
    #version 330 core
    out vec3 color;
    void main(){
      color = vec3(1,0,0);
    }    
)";


int main(void)
{
    GLFWwindow* window = CreateWindow();

    Shader shader(vertexShader, fragmentShader);
    shader.Use();

    int width;
    int height;

    glfwGetWindowSize(window, &width, &height);
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * Model;

    shader.setMatrix("MVP", mvp);
    
    glClearColor(0.2, 0.2, 0.8, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}