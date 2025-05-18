//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>

#include "core/window.h"
#include "graphics/graphics_core.h"
#include "graphics/graphics_objects.h"

using ProtoCADCore::Window;
using ProtoCADGraphics::GraphicsInstance;

#endif //APPLICATION_H

class Application {
public:
    std::shared_ptr<GraphicsInstance> graphics_instance;
    std::shared_ptr<Window> window;

    std::vector<ProtoCADGraphics::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    ProtoCADGraphics::Mesh mesh;

    Application();

    void Initialize();
    void Run();
};