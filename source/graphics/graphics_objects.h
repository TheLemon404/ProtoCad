//
// Created by Osprey on 5/17/2025.
//

#pragma once

#ifndef GRAPHICS_OBJECTS_H
#define GRAPHICS_OBJECTS_H

#include <vulkan/vulkan_core.h>
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#endif //GRAPHICS_OBJECTS_H

namespace ProtoCADGraphics {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct Model {
        Mesh mesh;
        glm::mat4 transform;
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };
}