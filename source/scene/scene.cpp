//
// Created by Osprey on 5/18/2025.
//

#include "scene.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace ProtoCADScene {
    void Camera::UpdateMatrices() {
        view = glm::lookAt(position, target, up);
    }

    void Scene::Update() {
        camera.UpdateMatrices();
    }
}
