//
// Created by Osprey on 5/15/2025.
//

#include "application.h"

#include "core/input.h"
#include "core/logging.h"
#include "core/clock.h"
#include "glm/ext/matrix_transform.hpp"

using ProtoCADCore::Logging;

Application::Application(ApplicationGraphicsAPI api) {
    if (api == VULKAN) {
        window = std::make_shared<Window>("ProtoCAD Vulkan", 800, 600);
    }
    else if (api == OPENGL) {
        window = std::make_shared<Window>("ProtoCAD OpenGL", 800, 600);
    }

    graphics_instance = std::make_shared<GraphicsInstance>(api);
    m_graphicsAPI = api;
}

void Application::Initialize() {
    Logging::Log("initializing application");

    //temporary (debugging purposes)
    model = {{vertices, indices}, glm::identity<glm::mat4>()};
    camera = {};
    camera.fov = 80;
    scene = {};

    window->Initialize(m_graphicsAPI);

    scene.models.push_back(model);
    scene.camera = camera;

    graphics_instance->Initialize(window, std::make_shared<ProtoCADScene::Scene>(scene));

    guiLayer = std::make_shared<GUILayer>(graphics_instance->GetAPI(), graphics_instance->GetAPIType());
    guiLayer->Initialize(window);
}

void Application::Run() {
    Logging::Log("running application");

    while (!window->ShouldClose()) {
        window->Poll();

        //temporary (debugging purposes)
        scene.camera.UpdateMatrices();
        if (ProtoCADCore::Input::keyStates[GLFW_KEY_T] == GLFW_PRESS && model.mesh.vertices[0].color.x == 0.0f) {

            scene.models[0].mesh = ProtoCADGraphics::DefaultQuad{};

            graphics_instance->UpdateMesh( scene.models[0].mesh);
        }

        //render loop
        graphics_instance->BeginDrawFrame(std::make_shared<ProtoCADScene::Scene>(scene), guiLayer->GetViewportWindowSize());
        guiLayer->Draw();
        graphics_instance->EndDrawFrame();

        //temporary (debugging purposes)
        scene.models[0].transform = glm::rotate(scene.models[0].transform, (float)ProtoCADCore::Clock::GetDeltaTime(), glm::vec3(0, 0, 1));
    }

    graphics_instance->CleanUp();
    window->Close();
}