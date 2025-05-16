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

    void GraphicsInstance::Initialize(std::shared_ptr<ProtoCADCore::Window> window) {
        switch (m_API) {
            case VULKAN:
                m_currentAPI = std::make_shared<VulkanAPI>();
                ProtoCADCore::Logging::Log("creating vulkan API instance");
        }

        m_currentAPI->Initialize(window);
    }

    void GraphicsInstance::CleanUp() {
        ProtoCADCore::Logging::Log("cleaning up graphics");
        m_currentAPI->CleanUp();
    }

}
