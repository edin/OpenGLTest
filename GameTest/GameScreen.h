#pragma once

#include "BasicTypes.h"

class Window;

class GameScreen {
protected:
    Window*   window;
    Color     color{0.0, 0.0, 0.0};
    Rectangle placement{ 0.0, 0.0, 1.0, 1.0 };
    float     timeStep;
public:
    void SetWindow(Window* window) { this->window = window; }
    void SetColor(Color color) { this->color = color; }
    void SetPlacement(Rectangle placement) { this->placement = placement; }
    void SetTimeStep(float time) { this->timeStep = time; }


    Size GetScreenSize() {
        Size size;
        glfwGetWindowSize(window, &size.width, &size.height);
        return size;
    }


    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Render() {};
    virtual ~GameScreen() {};
};