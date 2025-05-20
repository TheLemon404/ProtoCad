//
// Created by Osprey on 5/15/2025.
//

#include "opengl/opengl_core.h"
#include "graphics_core.h"

#include "../core/logging.h"
#include "../core/window.h"
#include "vulkan/vulkan_core.h"

namespace ProtoCADGraphics {
    GraphicsInstance::GraphicsInstance(ApplicationGraphicsAPI API) {
        m_API = API;
        m_currentAPI = nullptr;
    }

    void GraphicsInstance::Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) {
        if (m_API == VULKAN) {
            m_currentAPI = std::make_shared<VulkanAPI>();
            ProtoCADCore::Logging::Log("creating vulkan API instance");
        }
        else if (m_API == OPENGL) {
            m_currentAPI = std::make_shared<OpenGLAPI>();
            ProtoCADCore::Logging::Log("creating opengl API instance");
        }

        m_currentAPI->Initialize(window, mesh);
    }

    void GraphicsInstance::UpdateMesh(Mesh mesh, MeshUpdateType updateType) {
        switch (updateType) {
            case UPDATE_VERTEX_BUFFER:
                UpdateVertexBuffer(mesh.vertices);
                break;
            case UPDATE_INDEX_BUFFER:
                UpdateIndexBuffer(mesh.indices);
                break;
            case UPDATE_ALL_BUFFERS:
                UpdateVertexBuffer(mesh.vertices);
                UpdateIndexBuffer(mesh.indices);
                break;
            default:
                UpdateVertexBuffer(mesh.vertices);
                UpdateIndexBuffer(mesh.indices);
                break;
        }
    }

    void GraphicsInstance::UpdateVertexBuffer(std::vector<Vertex> vertices) {
        if (m_API == VULKAN) {
            std::shared_ptr<VulkanAPI> vk = std::static_pointer_cast<VulkanAPI>(m_currentAPI);
            vk->UpdateVertexBuffer(vertices);
        }
        else if (m_API == OPENGL) {
            std::shared_ptr<OpenGLAPI> ogl = std::static_pointer_cast<OpenGLAPI>(m_currentAPI);
        }
    }

    void GraphicsInstance::UpdateIndexBuffer(std::vector<uint32_t> indices) {
        if (m_API == VULKAN) {
            std::shared_ptr<VulkanAPI> vk = std::static_pointer_cast<VulkanAPI>(m_currentAPI);
            vk->UpdateIndexBuffer(indices);
        }
        else if (m_API == OPENGL) {
            std::shared_ptr<OpenGLAPI> ogl = std::static_pointer_cast<OpenGLAPI>(m_currentAPI);
        }
    }

    void GraphicsInstance::BeginDrawFrame(Model model, glm::mat4 view, float fov) {
        m_currentAPI->BeginDrawFrame(model, view, fov);
    }

    void GraphicsInstance::EndDrawFrame() {
        m_currentAPI->EndDrawFrame();
    }


    void GraphicsInstance::CleanUp() {
        ProtoCADCore::Logging::Log("cleaning up graphics");
        m_currentAPI->CleanUp();
    }

}
