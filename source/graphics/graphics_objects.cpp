//
// Created by Osprey on 5/17/2025.
//

#include "graphics_objects.h"
namespace ProtoCADGraphics {
    Volume::Volume(int width, int height, int depth) : width(width), height(height), depth(depth){
        data.resize(width * height * depth);
        Cube();
    }

    void Volume::CreateSphere() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (glm::distance(glm::vec3(x,y,z), glm::vec3(width / 2, height / 2, depth / 2)) < 32) {
                        int index = x * height * depth + y * depth + z;
                        data[index] = glm::vec4(x/width,y/height,z/depth,1.0f);
                    }
                }
            }
        }
    }

    void Volume::Cube() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    int index = x * height * depth + y * depth + z;
                    data[index] = glm::vec4(x,y,z,1.0f);
                }
            }
        }
    }
}
