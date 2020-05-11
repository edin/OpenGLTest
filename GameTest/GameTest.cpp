#pragma comment(lib, "opengl32")

#include "TypeAliases.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"

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

InputController inputController;
Camera camera;

void OnFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputController.UpdateKeyStatus(key, scancode, action, mods);
}

class Screen : public GameScreen {
public:
    void Render() override {
        glBegin(GL_LINES);
        for (int i = -10; i <= 10; i++) {
            glVertex3f(i * 0.1, 0.0, -1.0);
            glVertex3f(i * 0.1, 0.0, 1.0);
        }
        for (int i = -10; i <= 10; i++) {
            glVertex3f(-1.0, 0.0, i * 0.1);
            glVertex3f(1.0, 0.0, i * 0.1);
        }
        glEnd();
    }
};

class Screen2 : public GameScreen {
public:
    void Render() override {
        glBegin(GL_LINES);
        for (int i = -10; i <= 10; i++) {
            glVertex3f(i * 0.1, 1.0, -1.0);
            glVertex3f(i * 0.1, 1.0, 1.0);
        }
        for (int i = -10; i <= 10; i++) {
            glVertex3f(-1.0, 1.0, i * 0.1);
            glVertex3f(1.0, 1.0, i * 0.1);
        }
        glEnd();
    }
};

int main(void)
{
    Window window(1600, 900, "Open GL Test");
    Shader shader(vertexShader, fragmentShader);
    shader.Bind();

    window.AddScreen(std::make_unique<Screen>());
    window.AddScreen(std::make_unique<Screen2>());

    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeCallback);
    glfwSetKeyCallback(window, OnKeyCallback);

    Size windowSize = window.GetSize();
    camera.SetPerspectiveProjection(45.0f, windowSize.aspectRatio(), 0.1f, 100.0f);

    glClearColor(0.2, 0.2, 0.8, 1.0);

    while (!window.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glm::vec3 direction = inputController.GetDirection(0.01);
        camera.Move(direction);

        shader.SetMatrix("MVP", camera.GetMVP());
        auto& screens = window.GetScreens();
        for (auto& screen : screens) {
            screen->Update();
            screen->Render();
        }

        window.SwapBuffers();
        glfwPollEvents();
    }
    glfwTerminate();
}
