#pragma once

#include "TypeAliases.h"
#include "Logger.h"
#include "GameScreen.h"
#include "InputController.h"
#include "BasicTypes.h"

namespace Nuts {
    class Window;
    void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void OnFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    class Application {
    private:
        std::vector<Window*> windows;

        Application() {}

    public:
        static Application& GetInstance() {
            static Application instance;
            return instance;
        }

        void AddWindow(Window* window) {
            windows.push_back(window);
        }

        void RemoveWindow(Window* window) {
            auto it = std::find(windows.begin(), windows.end(), window);
            if (it != windows.end()) {
                windows.erase(it);
            }
        }

        const std::vector<Window*>& GetWindows() const {
            return windows;
        }

        void Loop();
        Window* FindWindow(GLFWwindow* window);

        Application(Application const&) = delete;

        void operator=(Application const&) = delete;
    };

    class Window {
    private:
        GLFWwindow* window = nullptr;
        UniqueVector<GameScreen> screens;
    public:
        InputController input;

        Window(int width, int height, const std::string& title) {
            Application::GetInstance().AddWindow(this);

            if (!glfwInit()) {
                Logger::Error("Window", "Initialization failed (glfwInit)");
                return;
            }
            window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

            if (!window) {
                Logger::Error("Window", "Failed to create Window");
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent(window);

            glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeCallback);
            glfwSetKeyCallback(window, OnKeyCallback);

            GLenum err = glewInit();
            if (GLEW_OK != err) {
                Logger::Error("CreateWindow", "Error initializing GLEW");
            }
        }

        virtual ~Window() {
            Application::GetInstance().RemoveWindow(this);
            DestroyWindow();
        }

        operator GLFWwindow* () const {
            return window;
        }

        void DestroyWindow() {
            glfwDestroyWindow(window);
        }

        GLFWwindow* GetWindow() {
            return window;
        }

        bool ShouldClose() {
            return glfwWindowShouldClose(window) != 0;
        }

        Size GetSize() {
            Size size;
            glfwGetWindowSize(window, &size.width, &size.height);
            return size;
        }

        void UpdateScreens() {
            for (auto& screen : screens) {
                screen->Update();
                screen->Render();
            }
        }

        void SwapBuffers() {
            glfwSwapBuffers(window);
        }

        void AddScreen(std::unique_ptr<GameScreen> screen) {
            screen->SetWindow(this);
            screen->Initialize();
            screens.push_back(std::move(screen));
        }

        const UniqueVector<GameScreen>& GetScreens() {
            return screens;
        }

        void EnterLoop() {
            while (!ShouldClose())
            {
                UpdateScreens();
                SwapBuffers();
                glfwPollEvents();
            }
            glfwTerminate();
        }
    };

    void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* win = Application::GetInstance().FindWindow(window);
        if (win != nullptr) {
            win->input.UpdateKeyStatus(key, scancode, action, mods);
        }
    }

    void OnFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
        Window* win = Application::GetInstance().FindWindow(window);
        if (win != nullptr) {
            //TODO: Forward to update size method
        }
    }

    Window* Application::FindWindow(GLFWwindow* window) {
        for (Window* win : windows) {
            if (win->GetWindow() == window) {
                return win;
            }
        }
        return nullptr;
    }

    void Application::Loop() {
        while (windows.size() > 0)
        {
            for (auto window : windows)
            {
                if (!window->ShouldClose()) {
                    glfwMakeContextCurrent(window->GetWindow());
                    window->UpdateScreens();
                    window->SwapBuffers();
                }
                else {
                    RemoveWindow(window);
                    window->DestroyWindow();
                }
            }
            glfwPollEvents();
        }
        glfwTerminate();
    }
}