//
// Created by Osprey on 5/20/2025.
//

#include "GLFW/glfw3.h"
#include "clock.h"

#include <algorithm>

double ProtoCADCore::Clock::m_t = 0.0;
double ProtoCADCore::Clock::m_dt = 0.01;
double ProtoCADCore::Clock::m_frameTime = 0;
double ProtoCADCore::Clock::m_lastTime = 0;
double ProtoCADCore::Clock::m_accumulator = 0;
double ProtoCADCore::Clock::m_deltaTime = 0;

namespace ProtoCADCore {
    double Clock::GetDeltaTime() {
        return m_deltaTime;
    }

    void Clock::Tick() {
        double newTime = glfwGetTime();
        m_frameTime = newTime - m_lastTime;
        m_lastTime = newTime;

        if (m_frameTime >= 0.0) {
            m_deltaTime = std::ranges::min(m_frameTime, m_dt);
            m_frameTime -= m_deltaTime;
            m_t += m_deltaTime;
        }
    }
}
