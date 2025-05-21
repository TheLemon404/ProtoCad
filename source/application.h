//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>

#include "types.h"
#include "core/window.h"
#include "editor/gui_layer.h"
#include "graphics/graphics_core.h"
#include "graphics/graphics_objects.h"
#include "scene/environment.h"

using ProtoCADCore::Window;
using ProtoCADGraphics::GraphicsInstance;
using ProtoCADGUI::GUILayer;

#endif //APPLICATION_H

class Application {
public:
    std::shared_ptr<GraphicsInstance> graphics_instance;
    std::shared_ptr<Window> window;
    std::shared_ptr<GUILayer> guiLayer;
    ApplicationGraphicsAPI m_graphicsAPI;

    std::vector<ProtoCADGraphics::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2
    };

    ProtoCADGraphics::Model model;
    ProtoCADScene::Camera camera;

    Application(ApplicationGraphicsAPI api);

    void Initialize();
    void Run();
};