#pragma comment(lib, "opengl32")
#include "Nuts/Nuts.h"

using namespace Nuts;

const auto vertexShader = R"(
    #version 330 core
    layout(location = 0) in vec3 vPosition;
    layout(location = 1) in vec3 vertexColor;
    layout(location = 2) in vec3 vNormal;
    layout(location = 3) in vec2 texCoordIn;

    out vec3 fragmentColor; 
    out vec2 texCoord;

    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(vPosition, 1.0);
        fragmentColor = vertexColor;
        texCoord = texCoordIn;
    }
)";

const auto fragmentShader = R"(
    #version 330 core
    out vec4 color;
    in  vec3 fragmentColor;
    in  vec2 texCoord;

    uniform sampler2D texture1;

    void main(){
       color = texture(texture1, texCoord);
       //color = vec4(fragmentColor, 1);
    }
)";

class Screen1 : public GameScreen {
private:
    Camera camera;
    Shader shader{ vertexShader, fragmentShader };
    std::vector<uint> cubeIndices;
    std::unique_ptr<Mesh> cubeMesh;
    unsigned char* image;
    GLuint texture;
    float time = 0.0;
public:
    void Initialize() {
        auto size = GetScreenSize();
        auto ratio = size.AspectRatio();
        camera.SetPerspectiveProjection(45.0f, ratio, 0.1f, 100.0f);

        float x1 = 420.0 / 840.0;
        float y1 = 240.0 / 671.0;
        float x2 = 620.0 / 840.0;
        float y2 = 440.0 / 671.0;

        //  0----------1  ----u---- >
        //  |          | 
        //  |          | 
        //  |          | 
        //  2----------3----------8----------10----------12 
        //  |          |          |          |           |  
        //  |          |          |          |           |  
        //  |          |          |          |           |  
        //  4----------5----------9----------11----------13 
        //  |          |
        //  |          |
        //  |          |
        //  6----------7

        float sizef = 512.0;
        float u = 12.0f / sizef;
        float v = 25.0f / sizef;
        float bs = 121.5f / sizef;

        float u1 = u + bs;
        float u2 = u + 2 * bs;
        float u3 = u + 3 * bs;
        float u4 = u + 4 * bs;

        float v1 = v + bs;
        float v2 = v + 2 * bs;
        float v3 = v + 3 * bs;
        float v4 = v + 4 * bs;
                
        std::vector<Vertex> cube = {
            Vertex(glm::vec3(0.0f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u, v)),   //0
            Vertex(glm::vec3(0.0f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u1,v)),   //1
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u, v1)),  //2

            Vertex(glm::vec3(0.0f, 0.0f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u1, v1)), //3
            Vertex(glm::vec3(0.3f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u, v2)),  //4 
            Vertex(glm::vec3(0.3f, 0.0f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u1, v2)), //5 
            
            Vertex(glm::vec3(0.3f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u,  v3)), //6
            Vertex(glm::vec3(0.3f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u1, v3)), //7 
            Vertex(glm::vec3(0.0f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u2, v1)), //8 
            
            Vertex(glm::vec3(0.3f, 0.3f, 0.3f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u2, v2)), //9 
            Vertex(glm::vec3(0.0f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u3, v1)), //10
            Vertex(glm::vec3(0.3f, 0.3f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u3, v2)), //11
            
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u4, v1)), //12 
            Vertex(glm::vec3(0.3f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), glm::vec2(u4, v2)), //13 
        };

        std::vector<Texture> textures = {};

        cubeIndices = {
            0, 1, 3,
            3, 0, 2,
            2, 3, 5,
            5, 2, 4,
            4, 5, 7,
            7, 4, 6,
            3, 8, 9,
            9, 3, 5,
            8, 10, 11,
            11, 8, 9,
            10, 12, 13,
            13, 10, 11
        };

        cubeMesh = std::make_unique<Mesh>(cube, cubeIndices, textures);

        int width, height, channels;

        stbi_set_flip_vertically_on_load(false);
        image = stbi_load("Resources\\texture.png", &width, &height, &channels, STBI_rgb);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);

    }

    void Update() override {
        glm::vec3 direction = window->input.GetDirection(0.01);

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

        time = time + 0.001;

        Scissor();
        ClearColor();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_TEXTURE_2D);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        camera.ResetModel();
        shader.SetMVPMatrix(camera.GetMVP());

        glBindTexture(GL_TEXTURE_2D, 0);

        //TODO: Make helper function to draw grid
        glBegin(GL_LINES);
        for (int i = -100; i <= 100; i++) {
            glColor3f(1.0, 0, 0);
            glVertex3f(i * 0.1, 0.0, -10.0);
            glVertex3f(i * 0.1, 0.0, 10.0);
        }
        for (int i = -100; i <= 100; i++) {
            glVertex3f(-10.0, 0.0, i * 0.1);
            glVertex3f(10.0, 0.0, i * 0.1);
        }
        glEnd();

        auto identity = glm::mat4(1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.SetInt("texture1", 0);
      

        for (int z = -10; z <= 10; z+=1) {
            for (int x = -10; x <= 10; x+=1) {
                float xf = x * 0.3 - 0.15;
                float zf = z * 0.3 - 0.15;
                //float yf = (sin(xf+time) + cos(zf + time)) * 0.4;
                float yf = 0;
                int xd = 10 - abs(x);
                int zd = 10 - abs(z);

                if (xd < 5 && zd < 5) {
                    yf = 0.3;
                }

                glm::mat4 translate = glm::translate(identity, glm::vec3(xf, yf, zf));
                camera.SetModel(translate);
                shader.SetMVPMatrix(camera.GetMVP());
                cubeMesh->Draw(shader);
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