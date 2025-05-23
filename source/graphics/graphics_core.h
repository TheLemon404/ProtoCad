//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H

#include <memory>
#include "../core/window.h"
#include "graphics_objects.h"
#include "../types.h"
#include "../scene/scene.h"

#endif //GRAPHICS_CORE_H

namespace ProtoCADGraphics {
    enum MeshUpdateType {
        UPDATE_INDEX_BUFFER,
        UPDATE_VERTEX_BUFFER,
        UPDATE_ALL_BUFFERS
    };

    class GraphicsAPI {
    public:
        virtual ~GraphicsAPI() = default;

        virtual void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene) = 0;
        virtual void BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene> scene, glm::vec2 viewport) = 0;
        virtual void EndDrawFrame() = 0;
        virtual void CleanUp() = 0;

        virtual int GetViewportRenderTexture() = 0;

        virtual void UpdateMesh(std::shared_ptr<Mesh> mesh, MeshUpdateType updateType) = 0;
    };

    class GraphicsInstance
    {
    private:
        ApplicationGraphicsAPI m_API;
        std::shared_ptr<GraphicsAPI> m_currentAPI;

    public:
        GraphicsInstance(ApplicationGraphicsAPI API);

        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene);
        void BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene>, glm::vec2 viewport);
        void EndDrawFrame();
        void CleanUp();
        void UpdateMesh(std::shared_ptr<Mesh> mesh,  MeshUpdateType updateType = UPDATE_ALL_BUFFERS);
        ApplicationGraphicsAPI GetAPIType() { return m_API; }
        std::shared_ptr<GraphicsAPI> GetAPI() { return m_currentAPI; }
    };
}