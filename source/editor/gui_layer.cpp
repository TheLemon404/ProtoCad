//
// Created by Osprey on 5/18/2025.
//

#include "gui_layer.h"

#include "gui_window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "../core/logging.h"
#include "../graphics/opengl/opengl_core.h"

namespace ProtoCADGUI {

    void GUILayer::Initialize(std::shared_ptr<ProtoCADCore::Window> window) {
        p_window = window;

        //initialize imgui (api agnostic)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();

        if (p_graphicsAPIType == VULKAN){
            auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);

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
        }
        else if(p_graphicsAPIType == OPENGL) {
            ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
            ImGui_ImplOpenGL3_Init("#version 450");
        }

        ProtoCADCore::Logging::Log("Gui Layer Initialized");
    }

    void GUILayer::Draw() {
        if (p_graphicsAPIType == VULKAN){
            auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        else if (p_graphicsAPIType == OPENGL) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }

        ImGui::NewFrame();

        //dockspace
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        //viewport
        ImGui::Begin("viewport", nullptr, ImGuiWindowFlags_NoCollapse);
        {
            ImGui::BeginChild("render");

            m_viewportWindowSize.x = ImGui::GetContentRegionAvail().x;
            m_viewportWindowSize.y = ImGui::GetContentRegionAvail().y;

            ImGui::Image(
                (ImTextureID)p_graphicsApi->GetViewportRenderTexture(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

            ImGui::EndChild();
        }

        ImGui::End();

        ImGui::Render();

        if (p_graphicsAPIType == VULKAN) {
            auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkApi->GetCommandBuffers()[vkApi->currentFrame]);
        }
        else if (p_graphicsAPIType == OPENGL) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void GUILayer::CleanUp() {
        if (p_graphicsAPIType == VULKAN) {
            auto vkApi = std::static_pointer_cast<ProtoCADGraphics::VulkanAPI>(p_graphicsApi);
            ImGui_ImplVulkan_Shutdown();
        }
        else if (p_graphicsAPIType == OPENGL) {
            ImGui_ImplOpenGL3_Shutdown();
        }

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
