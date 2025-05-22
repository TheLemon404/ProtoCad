//
// Created by Osprey on 5/18/2025.
//

#include "scene.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace ProtoCADScene {
    void Camera::RotateAround(float angle, glm::vec3 axis, glm::vec3 originPoint) {
        // Create the transformation matrix
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -originPoint);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
        glm::mat4 inverseTranslationMatrix = glm::translate(glm::mat4(1.0f), originPoint);
        glm::mat4 finalTransformation = inverseTranslationMatrix * rotationMatrix * translationMatrix;

        // Apply the transformation to a point
        glm::vec4 originalPoint = glm::vec4(position, 1.0f); // Assuming the object's origin
        position = finalTransformation * originalPoint;
    }

    void Camera::UpdateMatrices() {
        view = glm::lookAt(position, target, up);
    }

    void Scene::Update() {
        camera.UpdateMatrices();
    }
}
