#pragma comment(lib, "opengl32")
#include "Nuts/Nuts.h"

using namespace Nuts;

const auto vertexShader = R"(
    #version 330 core
    layout(location = 0) in vec3 vPosition;
    layout(location = 1) in vec3 vertexColor;
    
    out vec3 fragmentColor; 

    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(vPosition, 1.0);
        fragmentColor = vertexColor;
    }
)";

const auto fragmentShader = R"(
    #version 330 core
    out vec3 color;
    in vec3 fragmentColor;
    void main(){
      color = fragmentColor;
    }
)";

class Screen1 : public GameScreen {
private:
    Camera camera;
    Shader shader{ vertexShader, fragmentShader };
    std::vector<uint> cubeIndices;
    std::unique_ptr<Mesh> cubeMesh;

public:
    void Initialize() {
        auto size = GetScreenSize();
        auto ratio = size.AspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);

        std::vector<Vertex> cube = {
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.3f, 0.0f, 0.0f), Color(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(0.3f, 0.3f, 0.0f), Color(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.3f, 0.0f), Color(0.0f, 1.0f, 1.0f)),

            Vertex(glm::vec3(0.0f, 0.0f, 0.3f), Color(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.3f, 0.0f, 0.3f), Color(1.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(0.3f, 0.3f, 0.3f), Color(1.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.3f, 0.3f), Color(1.0f, 1.0f, 1.0f)),
        };

        cubeIndices = {
            0, 1, 2,
            2, 0, 3,

            0, 4, 1,
            1, 4, 5,

            0, 3, 7,
            7, 0, 4,

            1, 2, 6,
            6, 1, 5,

            6, 5, 4,
            6, 4, 7,

            6, 7, 2,
            2, 7, 3
        };

        cubeMesh = std::make_unique<Mesh>(cube, cubeIndices);

        // --------------------------------------
        //        4---------------5 
        //       / |             /| 
        //      0 ------------- 1 |
        //      |  |            | |
        //      |  7------------|-6
        //      | /             |/
        //      3---------------2 
        // --------------------------------------
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
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        camera.ResetModel();
        shader.SetMVPMatrix(camera.GetMVP());

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

        auto identity = glm::mat4(1.f);

        for (int z = -3; z <= 3; z+=2) {
            for (int x = -3; x <= 3; x+=2) {
                float xf = x * 0.3 - 0.15;
                float zf = z * 0.3 - 0.15;

                glm::mat4 translate = glm::translate(identity, glm::vec3(xf, 0.f, zf));
                camera.SetModel(translate);
                shader.SetMVPMatrix(camera.GetMVP());
                cubeMesh->Draw();
            }
        }
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
    
    auto& screens = window.GetScreens();

    screens[0]->SetColor(Color{ 1.0, 1.0, 1.0 });

    //window.AddScreen(std::make_unique<Screen2>());
    /*
    auto& screens = window.GetScreens();
    screens[0]->SetColor(Color{ 0.2, 0.2, 1.0 });
    screens[1]->SetColor(Color{ 0.2, 0.2, 0.8 });

    screens[0]->SetPlacement(Rectangle(0.0, 0.5, 1.0, 1.0));
    screens[1]->SetPlacement(Rectangle(0.0, 0.0, 0.5, 1.0));
    */ 

    //Window window2(1600, 900, "Open GL Test 2");
    //window2.AddScreen(std::make_unique<Screen1>());
    //window2.AddScreen(std::make_unique<Screen2>());

    Application::GetInstance().Loop();
}