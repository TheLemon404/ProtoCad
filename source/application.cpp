//
// Created by Osprey on 5/15/2025.
//

#include "application.h"
#include "core/logging.h"

using ProtoCADCore::Logging;

Application::Application() {
    window = std::make_shared<Window>("ProtoCAD", 800, 600);
    graphics_instance = std::make_shared<GraphicsInstance>(ProtoCADGraphics::VULKAN);
}

void Application::Initialize() {
    Logging::Log("initializing application");
    window->Initialize();
    graphics_instance->Initialize(window);
}

void Application::Run() {
    Logging::Log("running application");
    while (!window->ShouldClose()) {
        window->Poll();
    }

    graphics_instance->CleanUp();
    window->Close();
}