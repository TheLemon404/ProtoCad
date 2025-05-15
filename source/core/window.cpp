//
// Created by Osprey on 5/15/2025.
//

#include "window.h"
#include "logging.h"

namespace ProtoCADCore {
    Window::Window(const char* title, unsigned int width, unsigned int height) {
        m_title = title;
        m_width = width;
        m_height = height;
        m_glfwWindow = nullptr;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    void Window::Initialize() {
        Logging::Log("opening window");
        glfwInit();
        m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
        glfwMakeContextCurrent(m_glfwWindow);

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    }

    bool Window::ShouldClose() {
        return glfwWindowShouldClose(m_glfwWindow);
    }

    void Window::Poll() {
        glfwPollEvents();
    }

    void Window::Close() {
        glfwDestroyWindow(m_glfwWindow);
        glfwTerminate();
    }
}