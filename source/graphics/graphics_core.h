//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H
#include "../core/window.h"

#endif //GRAPHICS_CORE_H

namespace ProtoCADGraphics {
    enum GraphicsAPIType
    {
        VULKAN,
    };

    class API {
    public:
        virtual void Initialize(ProtoCADCore::Window* window) = 0;
        virtual void CleanUp() = 0;
    };

    class GraphicsInstance
    {
    private:
        GraphicsAPIType m_API;
        API* m_currentAPI;

    public:
        GraphicsInstance(GraphicsAPIType API);

        void Initialize(ProtoCADCore::Window* window);
        void CleanUp();
    };
}