//
// Created by Osprey on 5/18/2025.
//

#pragma once

#ifndef GUI_LAYER_H
#define GUI_LAYER_H

#include "imgui_impl_vulkan.h"
#include "../graphics/graphics_core.h"
#include "GLFW/glfw3.h"
#include "../graphics/vulkan/vulkan_core.h"

#endif //GUI_LAYER_H

namespace ProtoCADGUI {
    class GUILayer {
    private:
        const std::shared_ptr<ProtoCADGraphics::GraphicsAPI> p_graphicsApi;
        const ProtoCADGraphics::GraphicsAPIType p_graphicsAPIType;

        std::shared_ptr<ProtoCADCore::Window> p_window;

        static void check_vk_result(VkResult err)
        {
            if (err == VK_SUCCESS)
                return;
            fprintf(stderr, "[vulkan] GUILAYER Error: VkResult = %d\n", err);
            if (err < 0)
                abort();
        }

    public:
        GUILayer(std::shared_ptr<ProtoCADGraphics::GraphicsAPI> api, ProtoCADGraphics::GraphicsAPIType apiType) : p_graphicsApi(api), p_graphicsAPIType(apiType) {};
        void Initialize(std::shared_ptr<ProtoCADCore::Window> window);
        void Draw();
        void CleanUp();
    };
}