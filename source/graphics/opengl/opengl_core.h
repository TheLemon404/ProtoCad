//
// Created by Osprey on 5/20/2025.
//

#pragma once

#ifndef GL_CORE_H
#define GL_CORE_H

#include <glad/glad.h>
#include "../graphics_core.h"

#endif //GL_CORE_H

namespace ProtoCADGraphics {
    class OpenGLAPI : public GraphicsAPI {
    private:
        GLFWwindow* p_window;

    public:
        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) override;
        void BeginDrawFrame(Model model, glm::mat4 view, float fov) override;
        void EndDrawFrame() override;
        void CleanUp() override;
    };
}