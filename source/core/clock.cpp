//
// Created by Osprey on 5/20/2025.
//

#include "GLFW/glfw3.h"
#include "clock.h"

double ProtoCADCore::Clock::m_deltaTime = 0;
double ProtoCADCore::Clock::m_lastTime = 0;

namespace ProtoCADCore {
    double Clock::GetDeltaTime() {
        return m_deltaTime;
    }

    void Clock::Tick() {
        m_deltaTime = glfwGetTime() - m_lastTime;
        m_lastTime = glfwGetTime();
    }
}
