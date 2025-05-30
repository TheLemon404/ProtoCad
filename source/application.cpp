//
// Created by Osprey on 5/15/2025.
//

#include "application.h"

#include "core/input.h"
#include "core/logging.h"
#include "core/clock.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"

using ProtoCADCore::Logging;

Application::Application(ApplicationGraphicsAPI api) {
    if (api == OPENGL) {
        m_window = std::make_shared<Window>("ProtoCAD OpenGL", 800, 600);
    }

    m_graphicsInstance = std::make_shared<GraphicsInstance>(api);
    m_graphicsAPI = api;
}

void Application::UpdateCameraPosition() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraForward = glm::normalize(scene.camera.target - scene.camera.position);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraForward));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));

    if (ProtoCADCore::Input::mouseButtonStates[GLFW_MOUSE_BUTTON_3] == GLFW_PRESS) {
        if (ProtoCADCore::Input::keyStates[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS || ProtoCADCore::Input::keyStates[GLFW_KEY_LEFT_SHIFT] == GLFW_REPEAT) {
            scene.camera.position += (cameraRight * ProtoCADCore::Input::mouseDelta.x + cameraUp * ProtoCADCore::Input::mouseDelta.y) / 200.0f;
            scene.camera.target += (cameraRight * ProtoCADCore::Input::mouseDelta.x + cameraUp * ProtoCADCore::Input::mouseDelta.y) / 200.0f;
        }
        else if (ProtoCADCore::Input::keyStates[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS || ProtoCADCore::Input::keyStates[GLFW_KEY_LEFT_CONTROL] == GLFW_REPEAT) {
            float distanceMagnifier = pow(glm::distance(scene.camera.target, scene.camera.position), 1.7f);
            scene.camera.position += ((cameraRight * ProtoCADCore::Input::mouseDelta.x + cameraForward * ProtoCADCore::Input::mouseDelta.y) / 200.0f) * distanceMagnifier;
            scene.camera.target += ((cameraRight * ProtoCADCore::Input::mouseDelta.x + cameraForward * ProtoCADCore::Input::mouseDelta.y) / 200.0f) * distanceMagnifier;
        }
        else {
            scene.camera.RotateAround(-ProtoCADCore::Input::mouseDelta.x / 500.0f, glm::vec3(0, 1, 0), scene.camera.target);
            scene.camera.RotateAround(ProtoCADCore::Input::mouseDelta.y / 500.0f, cameraRight, scene.camera.target);
            scene.camera.rotation.x = glm::clamp(scene.camera.rotation.x, -glm::pi<float>(), glm::pi<float>());
            scene.camera.rotation.z = glm::clamp(scene.camera.rotation.z, -glm::pi<float>(), glm::pi<float>());
        }
    }

    scene.camera.position += (scene.camera.position - scene.camera.target) * (-ProtoCADCore::Input::mouseScrollVector.y / 20.0f);
    scene.camera.othoZoomFactor += (-ProtoCADCore::Input::mouseScrollVector.y / 10.0f);
    scene.camera.othoZoomFactor = glm::clamp(scene.camera.othoZoomFactor, 0.000001f, 1000000.0f);
}

void Application::Initialize() {
    Logging::Log("initializing application");

    m_window->Initialize(m_graphicsAPI);

    scene = {};
    scene.volume = {128, 128, 128};
    scene.camera = {};
    scene.camera.fov = 80;

    m_graphicsInstance->Initialize(m_window, std::make_shared<ProtoCADScene::Scene>(scene));

    m_guiLayer = std::make_shared<GUILayer>(m_graphicsInstance->GetAPI(), m_graphicsInstance->GetAPIType());
    m_guiLayer->Initialize(m_window);
}

void Application::Run() {
    Logging::Log("running application");

    while (!m_window->ShouldClose()) {
        m_window->Poll();

        UpdateCameraPosition();
        scene.camera.UpdateMatrices();

        //render loop
        m_graphicsInstance->BeginDrawFrame(std::make_shared<ProtoCADScene::Scene>(scene), m_guiLayer->GetViewportWindowSize());
        m_guiLayer->Draw(scene);
        m_graphicsInstance->EndDrawFrame();

        ProtoCADCore::Input::Refresh();
    }

    m_graphicsInstance->CleanUp();
    m_window->Close();
}