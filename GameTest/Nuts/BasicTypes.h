#pragma once

#include "TypeAliases.h"

namespace Nuts {

    struct Color {
        float r;
        float g;
        float b;
        Color() {}
        Color(float r, float g, float b) : r(r), g(g), b(b) {}
    };

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoords;
        Color color;

        Vertex(glm::vec3 position, Color color) {
            this->position = position;
            this->color = color;
            this->textureCoords = glm::vec2();
            this->normal = glm::vec3();
        }
    };

    struct Texture {
    };

    struct Size {
        int width;
        int height;

        float AspectRatio() {
            return (float)width / float(height);
        }
    };

    struct ViewPort {
        int x;
        int y;
        int with;
        int height;
    };

    struct Rectangle {
        float top;
        float left;
        float right;
        float bottom;

        Rectangle(float top, float left, float right, float bottom) :
            top(top), left(left), right(right), bottom(bottom)
        {
        }

        float Width() {
            return right - left;
        }

        float Height() {
            return bottom - top;
        }

        float IsValid() {
            return Width() >= 0.0 && Height() >= 0.0;
        }

        float IsEmpty() {
            return Width() <= 0.0 && Height() <= 0.0;
        }

        ViewPort ToViewPort(Size screenSize) {
            ViewPort result;
            result.x = (int)(left * screenSize.width);
            result.y = (int)(top * screenSize.height);
            result.with = (int)(Width() * screenSize.width);
            result.height = (int)(Height() * screenSize.height);
            return result;
        }
    };
}