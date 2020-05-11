#pragma once

#include "TypeAliases.h"

class InputController {
private:
    int keyStatus[512] = { 0 };
public:
    bool IsValidKey(int key) const {
        return (key >= 0 && key < 512);
    }

    void UpdateKeyStatus(int key, int scancode, int action, int mods) {
        if (IsValidKey(key)) {
            switch (action) {
            case GLFW_PRESS: {
                keyStatus[key] = 1;
            }break;
            case GLFW_REPEAT: {
                keyStatus[key] = 2;
            }break;
            case GLFW_RELEASE: {
                keyStatus[key] = 0;
            }break;
            }
        }
    }

    bool IsKeyDown(int key) const {
        if (IsValidKey(key)) {
            return keyStatus[key] > 0;
        }
    }

    bool IsLeft()  const { return IsKeyDown(GLFW_KEY_LEFT); }
    bool IsRight() const { return IsKeyDown(GLFW_KEY_RIGHT); }
    bool IsUp()    const { return IsKeyDown(GLFW_KEY_UP); }
    bool IsDown()  const { return IsKeyDown(GLFW_KEY_DOWN); }

    glm::vec3 GetDirection(float scale = 1.0) {
        float x = 0.0;
        float y = 0.0;
        const float cos45deg = 0.70710678118;

        if (IsLeft()) { x = -1.0; }
        if (IsRight()) { x = 1.0; }
        if (IsUp()) { y = 1.0; }
        if (IsDown()) { y = -1.0; }

        if (x != 0.0 && y != 0.0) {
            x = x * cos45deg;
            y = y * cos45deg;
        }
        auto result = glm::vec3(x, y, 0.0);

        result = result * scale;

        return result;
    }
};
