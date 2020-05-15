#pragma comment(lib, "opengl32")
#include "Nuts/Nuts.h"

using namespace Nuts;

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
        auto size = GetScreenSize();
        auto ratio = size.AspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);
    }

    void Update() override {
        glm::vec3 direction = window->input.GetDirection(0.01);
        //camera.Move(direction);

        if (window->input.IsLeft()) {
            camera.Rotate(0.10);
        }
        if (window->input.IsRight()) {
            camera.Rotate(-0.1);
        }
        if (window->input.IsUp()) {
            camera.Zoom(0.001);
        }
        if (window->input.IsDown()) {
            camera.Zoom(-0.001);
        }
        shader.SetMatrix("MVP", camera.GetMVP());
    }

    void Render() override {
        shader.Bind();

        Scissor();
        ClearColor();

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
        auto size = GetScreenSize();
        auto ratio = size.AspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);
    }

    void Update() override {
        glm::vec3 direction = window->input.GetDirection(0.01);
        camera.Move(direction);
        shader.SetMatrix("MVP", camera.GetMVP());
    }

    void Render() override {
        shader.Bind();

        Scissor();
        ClearColor();

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
    Window window(1600, 900, "Open GL Test 1");

    window.AddScreen(std::make_unique<Screen1>());
    //window.AddScreen(std::make_unique<Screen2>());

    auto& screens = window.GetScreens();
    screens[0]->SetColor(Color{ 0.2, 0.2, 1.0 });
    //screens[1]->SetColor(Color{ 0.2, 0.2, 0.8 });

    screens[0]->SetPlacement(Rectangle(0.0, 0.0, 0.5, 1.0));
    //screens[1]->SetPlacement(Rectangle(0.0, 0.5, 1.0, 1.0));

    //Window window2(1600, 900, "Open GL Test 2");
    //window2.AddScreen(std::make_unique<Screen1>());
    //window2.AddScreen(std::make_unique<Screen2>());

    Application::GetInstance().Loop();
}