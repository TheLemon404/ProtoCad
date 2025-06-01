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
                    if (glm::distance(glm::vec3(x,y,z), glm::vec3(0.0)) < radius) {
                        int index = x * height * depth + y * depth + z;
                        glm::vec3 center = glm::vec3(width / 2, height / 2, depth / 2);
                        glm::vec3 vector = center - glm::vec3(x,y,z);
                        data[index] = glm::vec4(vector / radius,1.0f);
                    }
                }
            }
        }
    }

    void Volume::Fill(int w, int h, int d) {
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                for (int z = 0; z < d; z++) {
                    int index = x * height * depth + y * depth + z;
                    glm::vec3 center = glm::vec3(width / 2, height / 2, depth / 2);
                    glm::vec3 vector = center - glm::vec3(x,y,z);
                    data[index] = glm::vec4(vector / glm::vec3(width / 2, height / 2, depth / 2),1.0f);
                }
            }
        }
    }

    void Volume::Singularity(glm::vec3 center, float distortionStrength) {
        for (int x = 1; x < width; x++) {
            for (int y = 1; y < height; y++) {
                for (int z = 1; z < depth; z++) {
                    int index = x * height * depth + y * depth + z;
                    glm::vec3 vector = glm::normalize(center - glm::vec3((float)x/(float)width,(float)y/(float)height,(float)z/(float)depth));
                    float inverseSquareLaw = 1.0f / pow(glm::distance(center, glm::vec3((float)x/(float)width,(float)y/(float)height,(float)z/(float)depth)) / distortionStrength, 2);
                    data[index] = glm::vec4(vector * inverseSquareLaw,1.0f);
                }
            }
        }
    }
}
