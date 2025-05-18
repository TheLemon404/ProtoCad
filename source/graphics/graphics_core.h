//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H
#include <memory>
#include <vector>

#include "../core/window.h"

#endif //GRAPHICS_CORE_H

#include "graphics_objects.h"

namespace ProtoCADGraphics {
    enum GraphicsAPIType
    {
        VULKAN,
    };

    class GraphicsAPI {
    public:
        virtual void HandleWindowResize() = 0;
        virtual void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::vector<Vertex> vertices, std::vector<uint32_t> indices) = 0;
        virtual void DrawFrame(std::vector<Vertex> vertices, std::vector<uint32_t> indices) = 0;
        virtual void CleanUp() = 0;
    };

    class GraphicsInstance
    {
    private:
        GraphicsAPIType m_API;
        std::shared_ptr<GraphicsAPI> m_currentAPI;

    public:
        GraphicsInstance(GraphicsAPIType API);

        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
        void DrawFrame(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
        void CleanUp();
        void UpdateVertexBuffer(std::vector<Vertex> vertices);
    };
}