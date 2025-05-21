//
// Created by Osprey on 5/20/2025.
//

#include "opengl_core.h"

#include <minwindef.h>

#include "../../core/io.h"
#include "../../core/logging.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.inl"

namespace ProtoCADGraphics {
    void ShaderProgram::UploadUniformFloat(const char *uniformName, float value) {
        glUniform1f(glGetUniformLocation(id, uniformName), value);
    }

    void ShaderProgram::UploadUniformInt(const char *uniformName, int value) {
        glUniform1i(glGetUniformLocation(id, uniformName), value);
    }

    void ShaderProgram::UploadUniformVec2(const char *uniformName, glm::vec2 value) {
        glUniform2fv(glGetUniformLocation(id, uniformName), 1, glm::value_ptr(value));
    }

    void ShaderProgram::UploadUniformVec3(const char *uniformName, glm::vec3 value) {
        glUniform3fv(glGetUniformLocation(id, uniformName), 1, glm::value_ptr(value));
    }

    void ShaderProgram::UploadUniformVec4(const char *uniformName, glm::vec4 value) {
        glUniform4fv(glGetUniformLocation(id, uniformName), 1, glm::value_ptr(value));
    }

    void ShaderProgram::UploadUniformMat4(const char *uniformName, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(id, uniformName), 1, FALSE, glm::value_ptr(value));
    }

    void ShaderProgram::Load(std::shared_ptr<ShaderObject> vertexShader, std::shared_ptr<ShaderObject> fragmentShader) {
        this->vertexShader = vertexShader;
        this->fragmentShader = fragmentShader;

        glAttachShader(id, vertexShader->id);
        glAttachShader(id, fragmentShader->id);
        glLinkProgram(id);

        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            ProtoCADCore::Logging::Error(infoLog);
        }
    }

    void ShaderProgram::Use() {
        glUseProgram(id);
    }

    void ShaderProgram::CleanUp() {
        glDetachShader(id, vertexShader->id);
        glDetachShader(id, fragmentShader->id);
        glDeleteShader(vertexShader->id);
        glDeleteShader(fragmentShader->id);
        glDeleteProgram(id);
    }

    void ShaderObject::Load(std::string localPath) {
        const char* source = ProtoCADCore::IO::ReadFileGLSL(localPath);

        glShaderSource(id, 1, &source, NULL);
        glCompileShader(id);

        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            ProtoCADCore::Logging::Error(infoLog);
        }
    }

    void BufferObject::Delete() {
        glDeleteBuffers(1, &id);
    }

    void VBO::UploadData(std::vector<float> data) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }

    void VBO::CreateVertexAttribPointer(int location, int size, int type) {
        glVertexAttribPointer(location, size, type, GL_FALSE, size * sizeof(float), (void*)0);
        glEnableVertexAttribArray(location);
    }

    void IBO::UploadData(std::vector<uint32_t> data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), GL_STATIC_DRAW);
    }

    void VAO::Bind() {
        glBindVertexArray(id);
    }

    void VAO::Unbind() {
        glBindVertexArray(0);
    }

    VAO OpenGLAPI::CreateVAO() {
        VAO vao{};
        glGenVertexArrays(1, &vao.id);
        vao.Bind();
        return vao;
    }

    VBO OpenGLAPI::CreateVBO(int numVertices) {
        VBO vbo{};
        glGenBuffers(1, &vbo.id);
        vbo.vertexCount = numVertices;
        return vbo;
    }

    IBO OpenGLAPI::CreateIBO(int numIndices) {
        IBO ibo{};
        glGenBuffers(1, &ibo.id);
        ibo.indexCount = numIndices;
        return ibo;
    }

    std::vector<float> OpenGLAPI::ExtractVertexPositions(std::vector<Vertex> vertices) {
        std::vector<float> data;

        for(int i = 0; i < vertices.size(); i++) {
            data.push_back(vertices[i].position.x);
            data.push_back(vertices[i].position.y);
            data.push_back(vertices[i].position.z);
        }

        return data;
    }

    std::vector<float> OpenGLAPI::ExtractVertexColors(std::vector<Vertex> vertices) {
        std::vector<float> data;

        for(int i = 0; i < vertices.size(); i++) {
            data.push_back(vertices[i].color.r);
            data.push_back(vertices[i].color.g);
            data.push_back(vertices[i].color.b);
        }

        return data;
    }

    ShaderObject OpenGLAPI::CreateShader(ShaderType type) {
        ShaderObject shader = {};
        shader.type = type;
        switch (type) {
            case VERTEX_SHADER:
                shader.id = glCreateShader(GL_VERTEX_SHADER);
                break;
            case FRAGMENT_SHADER:
                shader.id = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            default:
                break;
        }
        return shader;
    }

    ShaderProgram OpenGLAPI::CreateProgram() {
        ShaderProgram shaderProgram = {};
        shaderProgram.id = glCreateProgram();
        return shaderProgram;
    }

    void OpenGLAPI::Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) {
        p_window = window;

        if (!gladLoadGL()) {
            ProtoCADCore::Logging::Error("failed to initialize glad");
        }

        m_vertexShader = CreateShader(VERTEX_SHADER);
        m_vertexShader.Load("./assets/shaders/opengl/unlit.vert");
        m_fragmentShader = CreateShader(FRAGMENT_SHADER);
        m_fragmentShader.Load("./assets/shaders/opengl/unlit.frag");

        m_shaderProgram = CreateProgram();
        m_shaderProgram.Load(std::shared_ptr<ShaderObject>(&m_vertexShader), std::shared_ptr<ShaderObject>(&m_fragmentShader));

        m_vao = CreateVAO();

        //vertices
        m_vbo = CreateVBO((int)mesh.vertices.size() * 3);
        m_vbo.UploadData(ExtractVertexPositions(mesh.vertices));
        m_vbo.CreateVertexAttribPointer(0, 3, GL_FLOAT);

        m_cbo = CreateVBO((int)mesh.vertices.size() * 3);
        m_cbo.UploadData(ExtractVertexColors(mesh.vertices));
        m_cbo.CreateVertexAttribPointer(1, 3, GL_FLOAT);

        m_ibo = CreateIBO((int)mesh.indices.size());
        m_ibo.UploadData(mesh.indices);

        m_vao.vbo = std::make_shared<VBO>(m_vbo);
        m_vao.ibo = std::make_shared<IBO>(m_ibo);
    }

    void OpenGLAPI::BeginDrawFrame(Model model, glm::mat4 view, float fov) {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shaderProgram.Use();

        m_shaderProgram.UploadUniformMat4("model", model.transform);
        m_shaderProgram.UploadUniformMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(fov), p_window->GetDimensions().x / (float) p_window->GetDimensions().y, 0.001f, 10000.0f);
        m_shaderProgram.UploadUniformMat4("projection", projection);

        glBindVertexArray(m_vao.id);

        glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
    }

    void OpenGLAPI::EndDrawFrame() {
        glfwSwapBuffers(p_window->GetGLFWWindow());
    }

    void OpenGLAPI::CleanUp() {

    }
}
