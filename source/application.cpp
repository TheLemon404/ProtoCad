//
// Created by Osprey on 5/15/2025.
//

#include "application.h"
#include "core/logging.h"

using ProtoCADCore::Logging;

Application::Application() {
    window = new ProtoCADCore::Window("ProtoCAD", 800, 600);
    graphics_instance = new GraphicsInstance(ProtoCADGraphics::VULKAN);
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