//
// Created by Osprey on 5/15/2025.
//

#include "application.h"

#include "core/input.h"
#include "core/logging.h"

using ProtoCADCore::Logging;

Application::Application() {
    window = std::make_shared<Window>("ProtoCAD", 800, 600);
    graphics_instance = std::make_shared<GraphicsInstance>(ProtoCADGraphics::VULKAN);
}

void Application::Initialize() {
    Logging::Log("initializing application");
    window->Initialize();
    graphics_instance->Initialize(window, vertices);
}

void Application::Run() {
    Logging::Log("running application");

    while (!window->ShouldClose()) {
        window->Poll();
        if (ProtoCADCore::Input::keyStates[GLFW_KEY_T] == GLFW_PRESS && vertices[0].m_color.x == 1.0f) {
            vertices[0] = {{0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}};
            graphics_instance->UpdateVertexBuffer(vertices);
            Logging::Log("key pressed");
        }
        graphics_instance->DrawFrame(vertices);
    }

    graphics_instance->CleanUp();
    window->Close();
}