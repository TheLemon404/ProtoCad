//
// Created by Osprey on 5/18/2025.
//

#include "gui_layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "../../dependencies/imoguizmo/imoguizmo.hpp"
#include "../core/clock.h"
#include "../core/logging.h"
#include "../graphics/opengl/opengl_core.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace ProtoCADGUI {
    void GUILayer::InitializeTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.TabRounding = 0.0f;
        style.TabBarTopline = 2.0f;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Tab] = ImVec4(0.f, 0.f, 0.f, 0.f);
    }

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

        InitializeTheme();
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

            ImGui::TextColored(ImVec4(0.2f, 0.4f, 8.0f, 1.0f), "-- DEBUG INFO --");
            std::string volumeRes = std::string("volume resolution: ") + std::to_string(scene.volume.width) + std::string("x") + std::to_string(scene.volume.width) + std::string("x") + std::to_string(scene.volume.depth);
            ImGui::Text(volumeRes.c_str());

            // todo -- implement a working fps and reformat this terrible code
            if (ProtoCADCore::Clock::GetFPS() < 40) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), std::to_string(ProtoCADCore::Clock::GetFPS()).c_str());
            }
            else if (ProtoCADCore::Clock::GetFPS() >= 40 && ProtoCADCore::Clock::GetFPS() < 60) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), std::to_string(ProtoCADCore::Clock::GetFPS()).c_str());
            }
            else if (ProtoCADCore::Clock::GetFPS() >= 60) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), std::to_string(ProtoCADCore::Clock::GetFPS()).c_str());
            }


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

            if (ImGui::Button("Re-Center Camera")) {
                scene.camera.target = glm::vec3(0,0,0);
                scene.camera.position = glm::vec3(2,2,2);
            }

            ImGui::EndGroup();
            ImGui::SetItemAllowOverlap();

            //orientation guizmo
            ImOGuizmo::SetRect(m_viewportWindowSize.x - 110, 60, 100.0f);
            glm::mat4 projection = glm::perspective(glm::radians(scene.camera.fov), (float)m_viewportWindowSize.y/m_viewportWindowSize.x, 0.001f, 10000.0f);

            if (!ImOGuizmo::DrawGizmo((float*)&scene.camera.view, (float*)&projection)){
                //maybe implement something here later
            }

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
