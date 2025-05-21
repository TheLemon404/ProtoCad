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

        void Delete();
    };

    struct VBO : BufferObject {
        int vertexCount;

        void UploadData(std::vector<float> data);
        void CreateVertexAttribPointer(int location, int size, int type);
    };

    struct IBO : BufferObject {
        int indexCount;

        void UploadData(std::vector<uint32_t> data);
    };

    struct VAO {
        unsigned int id;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<IBO> ibo;

        void Bind();
        void Unbind();
    };


    struct ShaderObject
    {
        unsigned int id;
        ShaderType type;

        void Load(std::string localPath);
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

        //shaders
        ShaderObject m_vertexShader;
        ShaderObject m_fragmentShader;
        ShaderProgram m_shaderProgram;

        ShaderObject CreateShader(ShaderType type);
        ShaderProgram CreateProgram();

    public:
        void OnWindowResized(int width, int height);

        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) override;
        void BeginDrawFrame(Model model, glm::mat4 view, float fov) override;
        void EndDrawFrame() override;
        void CleanUp() override;
    };
}