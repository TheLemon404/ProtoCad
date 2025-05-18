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

    void GraphicsInstance::Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
        switch (m_API) {
            case VULKAN:
                m_currentAPI = std::make_shared<VulkanAPI>();
                ProtoCADCore::Logging::Log("creating vulkan API instance");
        }

        m_currentAPI->Initialize(window, vertices, indices);
    }

    void GraphicsInstance::UpdateVertexBuffer(std::vector<Vertex> vertices) {
        std::shared_ptr<VulkanAPI> vk = std::static_pointer_cast<VulkanAPI>(m_currentAPI);
        vk->UpdateVertexBuffer(vertices);
    }

    void GraphicsInstance::DrawFrame(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
        m_currentAPI->DrawFrame(vertices, indices);
    }

    void GraphicsInstance::CleanUp() {
        ProtoCADCore::Logging::Log("cleaning up graphics");
        m_currentAPI->CleanUp();
    }

}
