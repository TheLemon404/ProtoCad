//
// Created by Osprey on 5/20/2025.
//

#include "opengl_core.h"

#include "../../core/logging.h"

namespace ProtoCADGraphics {
    void OpenGLAPI::Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) {
        p_window = window->GetGLFWWindow();

        if (!gladLoadGL()) {
            ProtoCADCore::Logging::Error("failed to initialize glad");
        }
    }

    void OpenGLAPI::BeginDrawFrame(Model model, glm::mat4 view, float fov) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLAPI::EndDrawFrame() {
        glfwSwapBuffers(p_window);
    }

    void OpenGLAPI::CleanUp() {

    }
}
