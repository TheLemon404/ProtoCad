//
// Created by Osprey on 5/15/2025.
//

#include "window.h"

#include <iostream>

#include "logging.h"
#include "input.h"
#include "clock.h"

namespace ProtoCADCore {
    Window::Window(const char* title, unsigned int width, unsigned int height) {
        m_title = title;
        m_width = width;
        m_height = height;
        m_glfwWindow = nullptr;
    }

    void Window::GlfwErrorCallback(int code, const char* description)
    {
        std::cerr << "GLFW Error " << code << ": " << description << std::endl;
    }

    void Window::GlfwWindowResizedCallback(GLFWwindow *window, int width, int height) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->m_width = width;
        win->m_height = height;

        if (win->m_api == OPENGL) {
            glViewport(0, 0, width, height);
        }
    }


    void Window::Initialize(ApplicationGraphicsAPI api) {
        m_api = api;

        Logging::Log("opening window");
        glfwInit();

        glfwSetErrorCallback(Window::GlfwErrorCallback);

        if (api == VULKAN) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

        if(api == OPENGL) {
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwWindowHint(GLFW_SAMPLES, 1);
            glfwSwapInterval(0);
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwSetWindowUserPointer(m_glfwWindow, this);
        glfwSetWindowSizeCallback(m_glfwWindow, GlfwWindowResizedCallback);

        //input callbacks
        Input::Initialize();

        glfwSetKeyCallback(m_glfwWindow, Input::GLFWKeyCallback);
        glfwSetMouseButtonCallback(m_glfwWindow, Input::GLFWMouseButtonCallback);
        glfwSetScrollCallback(m_glfwWindow, Input::GLFWScrollCallback);
        glfwSetCursorPosCallback(m_glfwWindow, Input::GLFWSetCursorPosCallback);

        glfwMakeContextCurrent(m_glfwWindow);

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    }

    bool Window::ShouldClose() {
        return glfwWindowShouldClose(m_glfwWindow);
    }

    void Window::Poll() {
        glfwPollEvents();
        Clock::Tick();
    }

    void Window::Close() {
        glfwDestroyWindow(m_glfwWindow);
        glfwTerminate();
    }
}
