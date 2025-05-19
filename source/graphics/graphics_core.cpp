//
// Created by Osprey on 5/15/2025.
//

#include "graphics_core.h"

#include "../core/logging.h"
#include "../core/window.h"
#include "vulkan/vulkan_core.h"

namespace ProtoCADGraphics {
    GraphicsInstance::GraphicsInstance(GraphicsAPIType API) {
        m_API = API;
        m_currentAPI = nullptr;
    }

    void GraphicsInstance::Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) {
        switch (m_API) {
            case VULKAN:
                m_currentAPI = std::make_shared<VulkanAPI>();
                ProtoCADCore::Logging::Log("creating vulkan API instance");
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
        std::shared_ptr<VulkanAPI> vk = std::static_pointer_cast<VulkanAPI>(m_currentAPI);
        vk->UpdateVertexBuffer(vertices);
    }

    void GraphicsInstance::UpdateIndexBuffer(std::vector<uint32_t> indices) {
        std::shared_ptr<VulkanAPI> vk = std::static_pointer_cast<VulkanAPI>(m_currentAPI);
        vk->UpdateIndexBuffer(indices);
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
