//
// Created by Osprey on 5/15/2025.
//

#include "application.h"

#include "core/input.h"
#include "core/logging.h"
#include "glm/ext/matrix_transform.hpp"

using ProtoCADCore::Logging;

Application::Application() {
    window = std::make_shared<Window>("ProtoCAD", 800, 600);
    graphics_instance = std::make_shared<GraphicsInstance>(ProtoCADGraphics::VULKAN);
}

void Application::Initialize() {
    Logging::Log("initializing application");
    model = {{vertices, indices}, glm::identity<glm::mat4>()};
    camera = {};
    window->Initialize();
    graphics_instance->Initialize(window, model.mesh);

    guilayer = std::make_shared<GUILayer>(graphics_instance->GetAPI(), graphics_instance->GetAPIType());
    guilayer->Initialize(window);
}

void Application::Run() {
    Logging::Log("running application");

    while (!window->ShouldClose()) {
        camera.UpdateMatrices();
        window->Poll();
        if (ProtoCADCore::Input::keyStates[GLFW_KEY_T] == GLFW_PRESS && model.mesh.vertices[0].color.x == 0.0f) {

            model.mesh.vertices = {
                {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
            };

            model.mesh.indices = {
                0, 1, 2, 2, 3, 0
            };

            graphics_instance->UpdateMesh(model.mesh);
        }
        graphics_instance->BeginDrawFrame(model, camera.view, 45);
        guilayer->Draw();
        graphics_instance->EndDrawFrame();
    }

    graphics_instance->CleanUp();
    window->Close();
}