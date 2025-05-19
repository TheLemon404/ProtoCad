//
// Created by Osprey on 5/18/2025.
//

#include "gui_layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "../core/logging.h"

namespace ProtoCADGUI {

    void GUILayer::Initialize(std::shared_ptr<ProtoCADCore::Window> window) {
        p_window = window;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        switch (p_graphicsAPIType) {
            case ProtoCADGraphics::VULKAN:
                auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
                ImGui::StyleColorsDark();

                ImGui_ImplGlfw_InitForVulkan(window->GetGLFWWindow(), true);

                ImGui_ImplVulkan_InitInfo init_info = {};
                init_info.Instance = vkApi->GetInstance();
                init_info.PhysicalDevice = vkApi->GetPhysicalDevice();
                init_info.Device = vkApi->GetDevice();
                init_info.QueueFamily = vkApi->FindQueueFamilies(vkApi->GetPhysicalDevice()).graphicsFamily.value();
                init_info.Queue = vkApi->GetGraphicsQueue();
                init_info.PipelineCache = VK_NULL_HANDLE;
                init_info.DescriptorPool = vkApi->GetDescriptorPool();
                init_info.Allocator = nullptr;
                init_info.MinImageCount = 2;
                init_info.ImageCount = vkApi->GetImageCount();
                init_info.CheckVkResultFn = check_vk_result;
                init_info.RenderPass = vkApi->GetCurrentPipeline()->GetRenderPass();

                ImGui_ImplVulkan_Init(&init_info);

                ImGui_ImplVulkan_CreateFontsTexture();

                ProtoCADCore::Logging::Log("Gui Layer Initialized");
        }
    }

    void GUILayer::Draw() {
        switch (p_graphicsAPIType) {
            case ProtoCADGraphics::VULKAN:
                auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);

                ImGui_ImplVulkan_NewFrame();
                ImGui_ImplGlfw_NewFrame();

                ImGui::NewFrame();
                ImGui::ShowDemoWindow();
                ImGui::Render();

                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkApi->GetCommandBuffers()[vkApi->currentFrame]);
        }
    }

    void GUILayer::CleanUp() {
        auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
