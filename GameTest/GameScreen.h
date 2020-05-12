#pragma once

class Window;

class GameScreen {
protected:
    Window* window;
public:
    void SetWindow(Window* window) { this->window = window; }

    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Render() {};
    virtual ~GameScreen() {};
};
