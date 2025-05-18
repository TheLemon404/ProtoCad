//
// Created by Osprey on 5/17/2025.
//

#pragma once

#ifndef GRAPHICS_OBJECTS_H
#define GRAPHICS_OBJECTS_H
#include <vulkan/vulkan_core.h>

#endif //GRAPHICS_OBJECTS_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace ProtoCADGraphics {
    struct Vertex {
        glm::vec2 m_position;
        glm::vec3 m_color;
    };
}