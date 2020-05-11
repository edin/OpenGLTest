// Installed using vcpkg:
// vcpkg install glew
// vcpkg install glm
// vcpkg install glfw3

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#pragma comment(lib, "opengl32")

bool isLeft = false;
bool isRight = false;

void OnFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //SetupViewport(window);
}

void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        isLeft  = (key == GLFW_KEY_LEFT);
        isRight = (key == GLFW_KEY_RIGHT);
    }
    else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) {
            isLeft = false;
        }
        if (key == GLFW_KEY_RIGHT) {
            isRight = false;
        }
    }
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
    glfwSetKeyCallback(window, OnKeyCallback);

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

    glm::vec3 cameraPosition(4, 3, 3);
    glm::mat4 View = glm::lookAt(
        cameraPosition,
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * Model;

    shader.setMatrix("MVP", mvp);
    
    glClearColor(0.2, 0.2, 0.8, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        bool updateMVP = false;
        glClear(GL_COLOR_BUFFER_BIT);

        if (isLeft) {
            cameraPosition.x -= 0.1;
            updateMVP = true;
        }

        if (isRight) {
            cameraPosition.x += 0.1;
            updateMVP = true;
        }

        if (updateMVP) {
            glm::mat4 View = glm::lookAt(
                cameraPosition,
                glm::vec3(0, 0, 0), 
                glm::vec3(0, 1, 0)  
            );
            glm::mat4 Model = glm::mat4(1.0f);
            glm::mat4 mvp = Projection * View * Model;
            shader.setMatrix("MVP", mvp);
        }

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