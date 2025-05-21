//
// Created by Osprey on 5/19/2025.
//

#include "gui_window.h"

#include "imgui.h"

namespace ProtoCADGUI {
    void Viewport::Draw(unsigned int sceneTexture) {
        ImGui::Begin("viewport");
        {
            ImGui::BeginChild("GameRender");

            float width = ImGui::GetContentRegionAvail().x;
            float height = ImGui::GetContentRegionAvail().y;

            ImGui::Image(
                (ImTextureID)sceneTexture,
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
