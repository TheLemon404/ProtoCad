//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
#include "core/window.h"
#include "graphics/graphics_core.h"

using ProtoCADCore::Window;
using ProtoCADGraphics::GraphicsInstance;

#endif //APPLICATION_H

class Application {
public:
    GraphicsInstance* graphics_instance;
    Window* window;

    Application();

    void Initialize();
    void Run();
};