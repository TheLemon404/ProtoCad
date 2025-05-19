//
// Created by Osprey on 5/18/2025.
//

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#endif //CAMERA_H

namespace ProtoCADScene {
    struct Camera {
        glm::vec3 target;
        glm::vec3 position = {2.0f, 2.0f, 2.0f};
        glm::vec3 rotation;
        glm::vec3 up = {0.0f, 0.0f, 1.0f};
        glm::mat4 view;

        void UpdateMatrices();
    };

    class Environment {
    public:
        Camera camera;

        void Update();
    };
}