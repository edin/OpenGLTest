#pragma once

#include "TypeAliases.h"
#include "Logger.h"
#include "GameScreen.h"

struct Size {
    int width;
    int height;

    float aspectRatio() {
        return (float)width / float(height);
    }
};

class Window {
private:
    GLFWwindow* window = nullptr;
    UniqueVector<GameScreen> screens;
public:
    Window(int width, int height, const std::string& title) {
        if (!glfwInit()) {
            Logger::error("Window", "Initialization failed (glfwInit)");
            return;
        }
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (!window) {
            Logger::error("Window", "Failed to create Window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        GLenum err = glewInit();
        if (GLEW_OK != err) {
            Logger::error("CreateWindow", "Error initializing GLEW");
        }
    }

    operator GLFWwindow* () const {
        return window;
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
        screens.push_back(std::move(screen));
    }

    const UniqueVector<GameScreen>& GetScreens() {
        return screens;
    }
};
