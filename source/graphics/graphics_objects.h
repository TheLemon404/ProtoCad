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
#include <memory>

#endif //GRAPHICS_OBJECTS_H

namespace ProtoCADGraphics {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    struct Mesh {
        unsigned int id;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Mesh() { id = rand(); }
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

    struct DefaultQuad : Mesh {
        DefaultQuad() {
            vertices = {
                {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
                {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
            };

            indices = {
                0, 1, 2, 2, 3, 0
            };
        }
    };

    struct VoxelData {
        glm::vec4 color;
    };

    struct Node {
        bool isLeaf;
        std::shared_ptr<Node> children[8];
        VoxelData data;
    };

    struct Volume {
        int width, height, depth;

        glm::vec3 minBounds = glm::vec3(-1);
        glm::vec3 maxBounds = glm::vec3(1);

        Volume(int width = 32, int height = 32, int depth = 32);

        void CreateSphere(float radius);
        void Fill(int w = 32, int h = 32, int d = 32);
        void Singularity(glm::vec3 center, float distortionStrength);

        std::vector<glm::vec4> data;

    private:
        int m_textureID;
    };
}