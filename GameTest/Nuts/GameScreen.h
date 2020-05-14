#pragma once

#include "BasicTypes.h"

namespace Nuts {
    class Window;

    class GameScreen {
    protected:
        Window* window;
        Color   color{ 0.0, 0.0, 0.0 };
        Rectangle placement{ 0.0, 0.0, 1.0, 1.0 };
        float     timeStep;
    public:
        void SetWindow(Window* window) { this->window = window; }
        void SetColor(Color color) { this->color = color; }
        void SetPlacement(Rectangle placement) { this->placement = placement; }
        void SetTimeStep(float time) { this->timeStep = time; }

        void ClearColor() {
            glClearColor(color.r, color.g, color.b, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        Size GetScreenSize();
        ViewPort GetScreenViewPort();

        void Scissor() {
            ViewPort viewPort = GetScreenViewPort();
            glViewport(viewPort.x, viewPort.y, viewPort.with, viewPort.height);
            glEnable(GL_SCISSOR_TEST);
            glScissor(viewPort.x, viewPort.y, viewPort.with, viewPort.height);
        }

        virtual void Initialize() {};
        virtual void Update() {};
        virtual void Render() {};
        virtual ~GameScreen() {};
    };
}