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

        if(p_graphicsAPIType == OPENGL) {
            ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
            ImGui_ImplOpenGL3_Init("#version 450");
        }

        ProtoCADCore::Logging::Log("Gui Layer Initialized");
    }

    void GUILayer::Draw(ProtoCADScene::Scene& scene) {
        if (p_graphicsAPIType == OPENGL) {
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

            ImGui::SetItemAllowOverlap();

            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::BeginGroup();
            const char* perspectiveLabel = scene.camera.projection_mode == ProtoCADScene::PERSPECTIVE ? "Perspective" : "Orthographic";
            if (ImGui::Button(perspectiveLabel)) {
                if (scene.camera.projection_mode == ProtoCADScene::ORTHOGRAPHIC) {
                    ProtoCADScene::Camera cam = scene.camera;
                    cam.projection_mode = ProtoCADScene::PERSPECTIVE;
                    scene.camera = cam;
                }
                else if (scene.camera.projection_mode == ProtoCADScene::PERSPECTIVE) {
                    scene.camera.projection_mode = ProtoCADScene::ORTHOGRAPHIC;
                }
            }
            ImGui::EndGroup();
            ImGui::SetItemAllowOverlap();

            ImGui::EndChild();
        }

        ImGui::End();

        ImGui::Render();

        if (p_graphicsAPIType == OPENGL) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void GUILayer::CleanUp() {
        if (p_graphicsAPIType == OPENGL) {
            ImGui_ImplOpenGL3_Shutdown();
        }

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
