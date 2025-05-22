//
// Created by Osprey on 5/15/2025.
//

#include "opengl/opengl_core.h"
#include "graphics_core.h"

#include "../core/logging.h"
#include "../core/window.h"

namespace ProtoCADScene {
    class Scene;
}

namespace ProtoCADGraphics {
    GraphicsInstance::GraphicsInstance(ApplicationGraphicsAPI API) {
        m_API = API;
        m_currentAPI = nullptr;
    }

    void GraphicsInstance::Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene) {
        if (m_API == OPENGL) {
            m_currentAPI = std::make_shared<OpenGLAPI>();
            ProtoCADCore::Logging::Log("creating opengl API instance");
        }

        m_currentAPI->Initialize(window, scene);
    }

    void GraphicsInstance::UpdateMesh(std::shared_ptr<Mesh> mesh, MeshUpdateType updateType) {
        m_currentAPI->UpdateMesh(mesh, updateType);
    }

    void GraphicsInstance::BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene> scene, glm::vec2 viewport) {
        m_currentAPI->BeginDrawFrame(scene, viewport);
    }

    void GraphicsInstance::EndDrawFrame() {
        m_currentAPI->EndDrawFrame();
    }


    void GraphicsInstance::CleanUp() {
        ProtoCADCore::Logging::Log("cleaning up graphics");
        m_currentAPI->CleanUp();
    }

}
