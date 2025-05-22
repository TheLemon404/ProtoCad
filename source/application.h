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
#include "scene/scene.h"

using ProtoCADCore::Window;
using ProtoCADGraphics::GraphicsInstance;
using ProtoCADGUI::GUILayer;

#endif //APPLICATION_H

class Application {
private:
    std::shared_ptr<GraphicsInstance> m_graphicsInstance;
    std::shared_ptr<Window> m_window;
    std::shared_ptr<GUILayer> m_guiLayer;
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
    ProtoCADScene::Scene scene;

    void UpdateCameraPosition();

public:
    Application(ApplicationGraphicsAPI api);

    void Initialize();
    void Run();
};