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

class Screen1 : public GameScreen {
private:
    Camera camera;
    Shader shader{ vertexShader, fragmentShader };
public:

    void Initialize() {
        auto size = window->GetSize();
        auto ratio = size.aspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);
    }

    void Update() override {
        glm::vec3 direction = window->input.GetDirection(0.01);
        camera.Move(direction);
        shader.SetMatrix("MVP", camera.GetMVP());
    }

    void Render() override {

        shader.Bind();

        glClearColor(0.2, 0.2, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

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
private:
    Camera camera;
    Shader shader{ vertexShader, fragmentShader };
public:
    void Initialize() {
        auto size = window->GetSize();
        auto ratio = size.aspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);
    }

    void Update() override {
        glm::vec3 direction = window->input.GetDirection(0.01);
        camera.Move(direction);
        shader.SetMatrix("MVP", camera.GetMVP());
    }

    void Render() override {
        shader.Bind();
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

    window.AddScreen(std::make_unique<Screen1>());
    window.AddScreen(std::make_unique<Screen2>());

    window.EnterLoop();
}
