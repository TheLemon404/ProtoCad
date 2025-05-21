//
// Created by Osprey on 5/20/2025.
//

#pragma once

#ifndef GL_CORE_H
#define GL_CORE_H

#include <glad/glad.h>
#include "../graphics_core.h"

#endif //GL_CORE_H

namespace ProtoCADGraphics {
    enum ShaderType {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
    };

    enum BufferType {
        VERTEX_BUFFER,
        INDEX_BUFFER
    };

    struct BufferObject {
        unsigned int id;
        unsigned int size;

        virtual void Delete();
    };

    struct VBO : BufferObject {
        int vertexCount;

        void UploadData(std::vector<float> data);
        void CreateVertexAttribPointer(int location, int size, int type);

        void Delete() override;
    };

    struct IBO : BufferObject {
        int indexCount;

        void UploadData(std::vector<uint32_t> data);

        void Delete() override;
    };

    struct VAO {
        unsigned int id;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<VBO> cbo;
        std::shared_ptr<VBO> ubo;
        std::shared_ptr<IBO> ibo;

        void Bind();
        void Unbind();

        void Delete();
    };


    struct ShaderObject
    {
        unsigned int id;
        ShaderType type;

        void Load(std::string localPath);
    };

    struct Texture {
        unsigned int id;

        int width, height;

        void Bind();
        void Unbind();

        void Delete();
    };

    struct FrameBuffer {
        unsigned int id;

        void Rescale(int width, int height, std::shared_ptr<Texture> texture);

        void Bind();
        void Unbind();

        void Delete();
    };


    struct ShaderProgram {
        unsigned int id;

        std::shared_ptr<ShaderObject> vertexShader;
        std::shared_ptr<ShaderObject> fragmentShader;

        void UploadUniformFloat(const char* uniformName, float value);
        void UploadUniformInt(const char* uniformName, int value);
        void UploadUniformVec2(const char* uniformName, glm::vec2 value);
        void UploadUniformVec3(const char* uniformName, glm::vec3 value);
        void UploadUniformVec4(const char* uniformName, glm::vec4 value);
        void UploadUniformMat4(const char* uniformName, glm::mat4 value);

        void Load(std::shared_ptr<ShaderObject> vertexShader, std::shared_ptr<ShaderObject> fragmentShader);
        void Use();
        void CleanUp();
    };

    class OpenGLAPI : public GraphicsAPI {
    private:
        std::shared_ptr<ProtoCADCore::Window> p_window;

        //buffers
        VAO m_vao;
        VBO m_vbo;
        VBO m_cbo;
        IBO m_ibo;

        VAO CreateVAO();

        VBO CreateVBO(int numVertices);
        IBO CreateIBO(int numIndices);

        //vertex utils
        std::vector<float> ExtractVertexPositions(std::vector<Vertex> vertices);
        std::vector<float> ExtractVertexColors(std::vector<Vertex> vertices);
        std::vector<float> ExtractVertexTexCoords(std::vector<Vertex> vertices);

        //shaders
        ShaderObject m_vertexShader;
        ShaderObject m_fragmentShader;
        ShaderProgram m_shaderProgram;

        ShaderObject CreateShader(ShaderType type);
        ShaderProgram CreateProgram();

        //framebuffers
        FrameBuffer m_frameBuffer;
        Texture m_renderTexture;

        FrameBuffer CreateFrameBuffer();

        void CreateRenderedFrameBuffer();

        //textures
        Texture CreateTexture();

        //background assets
        DefaultQuad m_defaultQuad;
        VAO m_quadVao;
        VBO m_quadVbo;
        VBO m_quadCbo;
        VBO m_quadUbo;
        IBO m_quadIbo;

        ShaderObject m_screenSpaceVertexShader;
        ShaderObject m_checkeredFragmentShader;
        ShaderProgram m_checkeredProgram;

        ShaderObject m_gridVertexShader;
        ShaderObject m_gridFragmentShader;
        ShaderProgram m_gridProgram;

        void DrawGrid(glm::vec2 viewport, ProtoCADScene::Camera camera);
        void DrawCheckeredBackground(glm::vec2 viewport);

    public:
        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene) override;
        void BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene> scene, glm::vec2 viewport) override;
        void EndDrawFrame() override;
        void CleanUp() override;
        int GetViewportRenderTexture() override { return m_renderTexture.id; }

        void UpdateVertexBuffer(std::vector<Vertex> vertices);
        void UpdateIndexBuffer(std::vector<uint32_t> indices);

    };
}