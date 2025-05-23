//
// Created by Osprey on 5/18/2025.
//

#pragma once

#ifndef INPUT_H
#define INPUT_H
#include <map>

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#endif //INPUT_H

#define JUST_PRESSSED 500
#define JUST_RELEASED 600

namespace ProtoCADCore {
    class Input {
    public:
        static inline glm::vec2 mouseScrollVector;
        static inline glm::vec2 mousePosition;
        static inline glm::vec2 mouseDelta;

        static inline std::map<int, int> keyStates;
        static inline std::map<int, int> mouseButtonStates;

        static void Initialize();

        static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GLFWSetCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void Refresh();
    };
}