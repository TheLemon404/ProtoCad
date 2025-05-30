//
// Created by Osprey on 5/17/2025.
//

#include "graphics_objects.h"

#include "../core/logging.h"

namespace ProtoCADGraphics {
    Volume::Volume(int width, int height, int depth) : width(width), height(height), depth(depth){
        data.resize(width * height * depth);
    }

    void Volume::CreateSphere(float radius) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (glm::distance(glm::vec3(x,y,z), glm::vec3(width / 2, height / 2, depth / 2)) < radius) {
                        int index = x * height * depth + y * depth + z;
                        glm::vec3 center = glm::vec3(width / 2, height / 2, depth / 2);
                        glm::vec3 vector = center - glm::vec3(x,y,z);
                        data[index] = glm::vec4(vector / radius,1.0f);
                    }
                }
            }
        }
    }

    void Volume::Fill() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    int index = x * height * depth + y * depth + z;
                    glm::vec3 center = glm::vec3(width / 2, height / 2, depth / 2);
                    glm::vec3 vector = center - glm::vec3(x,y,z);
                    data[index] = glm::vec4(vector / glm::vec3(width / 2, height / 2, depth / 2),1.0f);
                }
            }
        }
    }
}
