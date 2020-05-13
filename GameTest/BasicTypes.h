#pragma once

#include "TypeAliases.h"

struct Color {
    float r;
    float g;
    float b;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

struct Texture {

};

struct Size {
    int width;
    int height;

    float aspectRatio() {
        return (float)width / float(height);
    }
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

    float width() {
        return right - left;
    }

    float height() {
        return bottom - top;
    }

    float isValid() {
        return width() >= 0.0 && height() >= 0.0;
    }

    float isEmpty() {
        return width() <= 0.0 && height() <= 0.0;
    }
};