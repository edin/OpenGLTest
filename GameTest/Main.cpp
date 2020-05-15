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
    GLuint cubeBuffer;
    GLuint elementbuffer;
    std::vector<GLuint> cubeIndices;
public:
    void Initialize() {
        auto size = GetScreenSize();
        auto ratio = size.AspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);

        static const Vertex cube[] = {
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(0.3f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.3f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f)),

            Vertex(glm::vec3(0.0f, 0.0f, 0.3f), Color(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(0.3f, 0.0f, 0.3f), Color(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.3f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f)),
        };

        // --------------------------------------
        //        4---------------5 
        //       / |             /| 
        //      0 ------------- 1 |
        //      |  |            | |
        //      |  7------------|-6
        //      | /             |/
        //      3---------------2 
        // --------------------------------------

        cubeIndices = { 
            0, 1, 2, 
            2, 0, 3, 

            0, 4, 1,
            1, 4, 5,

            0, 3, 7,
            7, 0, 4,

            6, 1, 2,
            2, 6, 3,

            6, 4, 1,
            1, 4, 5,

            6, 3, 7,
            7, 6, 4,
        };

        glGenBuffers(1, &cubeBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), &cubeIndices[0], GL_STATIC_DRAW);
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

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, (void*)0);
        glDisableVertexAttribArray(0);
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