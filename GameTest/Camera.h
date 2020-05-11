#pragma once

#include "TypeAliases.h"

class Camera {
public:
    glm::vec3 position{ 0, 0, 3 };
    glm::vec3 lookAt{ 0,0,0 };
    glm::vec3 up{ 0,1,0 };

    glm::mat4 Projection;
    glm::mat4 Model;

    Camera() {
        Model = glm::mat4(1.0f);
        Projection = glm::mat4(1.0f);
    }

    void SetPerspectiveProjection(float angleDegrees, float aspect, float zNear, float zFar) {
        Projection = glm::perspective(glm::radians(angleDegrees), aspect, zNear, zFar);
    }

    void SetOrthoProjection() {
        //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    }

    glm::mat4 GetLookAtMatrix() {
        glm::mat4 View = glm::lookAt(
            position,
            lookAt,
            up
        );
        return View;
    }

    void Move(glm::vec3 direction) {
        position = position + direction;
    }

    glm::mat4 GetMVP() {
        auto View = GetLookAtMatrix();
        return Projection * View * Model;
    }
};
