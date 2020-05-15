#pragma once

#include "TypeAliases.h"

namespace Nuts {
    class Camera {
    private:
        glm::vec3 position{ 0, 0, 3 };
        glm::vec3 lookAt{ 0, 0, 0 };
        glm::vec3 up{ 0,1,0 };
        glm::vec3 direction{ 0, 0, -1 };
        glm::vec3 rotationAxis{ 0,0,1 };

        glm::mat4 Projection;
        glm::mat4 Model;
        float distance = 3.0;
        float rotationDeg = 0.0;

        const double M_PI = 3.14159265358979323846;
    public:
        
        Camera() {
            Model = glm::mat4(1.0f);
            Projection = glm::mat4(1.0f);
            Update();
        }

        void Update() {
            position = lookAt - direction * distance;
        }

        void SetPosition(glm::vec3 value) {
            this->position = value;
            Update();
        }

        void SetDirection(glm::vec3 value) {
            this->direction = value;
            Update();
        }

        void SetUp(glm::vec3 value) {
            this->up = value;
            Update();
        }

        const glm::vec3 LookAt() const { return lookAt; }
        const glm::vec3 Position() const { return position; }
        const glm::vec3 Up() const { return up; }
        const glm::vec3 Direction() const { return direction; }
        const float Distance() const { return distance; }
        const float Rotation() const { return rotationDeg; }

        void SetPerspectiveProjection(float angleDegrees, float aspect, float zNear, float zFar) {
            Projection = glm::perspective(glm::radians(angleDegrees), aspect, zNear, zFar);
        }

        void SetOrthoProjection() {
            Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
        }

        void Zoom(float zoom) {
            distance += zoom;
            Update();
        }

        void SetDistance(float distance) {
            this->distance = distance;
        }

        void Rotate(float angleDeg) {
            this->rotationDeg += angleDeg;
            Update();
        }

        void SetRotation(float rotation) {
            this->rotationDeg = rotation;
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
            //glm::quat rotation = glm::angleAxis(glm::radians(rotationDeg), rotationAxis);
            

            return Projection * View * Model;
        }
    };
}