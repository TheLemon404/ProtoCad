//
// Created by Osprey on 5/20/2025.
//

#include "opengl_core.h"

#include <minwindef.h>

#include "../../core/io.h"
#include "../../core/logging.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
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

    void VBO::Delete() {
        BufferObject::Delete();
        vertexCount = 0;
    }

    void VAO::Delete() {
        glDeleteVertexArrays(1, &id);
    }

    void IBO::UploadData(std::vector<uint32_t> data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), GL_STATIC_DRAW);
    }

    void IBO::Delete(){
        BufferObject::Delete();
        indexCount = 0;
    }

    void VAO::Bind() {
        glBindVertexArray(id);
    }

    void VAO::Unbind() {
        glBindVertexArray(0);
    }

    void FrameBuffer::Rescale(int width, int height, std::shared_ptr<Texture> texture) {
        texture->Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
    }

    void FrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Delete() {
        glDeleteFramebuffers(1, &id);
    }

    void Texture::Bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Delete() {
        glDeleteTextures(1, &id);
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

    std::vector<float> OpenGLAPI::ExtractVertexTexCoords(std::vector<Vertex> vertices) {
        std::vector<float> data;

        for(int i = 0; i < vertices.size(); i++) {
            data.push_back(vertices[i].texCoord.x);
            data.push_back(vertices[i].texCoord.y);
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

    FrameBuffer OpenGLAPI::CreateFrameBuffer() {
        FrameBuffer frameBuffer{};
        glGenFramebuffers(1, &frameBuffer.id);
        frameBuffer.Bind();

        return frameBuffer;
    }

    Texture OpenGLAPI::CreateTexture() {
        Texture texture{};
        glGenTextures(1, &texture.id);
        texture.Bind();

        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
        texture.width = 1024;
        texture.height = 768;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return texture;
    }

    void OpenGLAPI::DrawCheckeredBackground(glm::vec2 viewport) {
        m_checkeredProgram.Use();

        m_checkeredProgram.UploadUniformVec2("resolution", viewport);
        m_checkeredProgram.UploadUniformFloat("darkGrey", 0.05f);
        m_checkeredProgram.UploadUniformFloat("lightGrey", 0.1f);

        glBindVertexArray(m_quadVao.id);
        glDrawElements(GL_TRIANGLES, m_defaultQuad.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    void OpenGLAPI::DrawGrid(glm::vec2 viewport, ProtoCADScene::Camera camera) {
        m_gridProgram.Use();

        glm::mat4 view = glm::lookAt(camera.position, camera.target, glm::vec3(0.0f, 1.0f, 0.0f));
        m_gridProgram.UploadUniformMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), viewport.x / viewport.y, 0.001f, 10000.0f);
        m_gridProgram.UploadUniformMat4("projection", projection);

        glBindVertexArray(m_quadVao.id);
        glDrawElements(GL_TRIANGLES, m_defaultQuad.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    void OpenGLAPI::Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene) {
        p_window = window;

        if (!gladLoadGL()) {
            ProtoCADCore::Logging::Error("failed to initialize glad");
        }

        glEnable(GL_CULL_FACE | GL_DEPTH_TEST);
        glCullFace(GL_BACK);

        //initialize framebuffers and render texture
        m_frameBuffer = CreateFrameBuffer();
        m_renderTexture = CreateTexture();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTexture.id, 0);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            ProtoCADCore::Logging::Error("failed to create framebuffer");
        }

        //initialize background assets
        m_defaultQuad = {};

        m_quadVao = CreateVAO();

        m_quadVbo = CreateVBO((int)m_defaultQuad.vertices.size() * 3);
        m_quadVbo.UploadData(ExtractVertexPositions(m_defaultQuad.vertices));
        m_quadVbo.CreateVertexAttribPointer(0, 3, GL_FLOAT);

        m_quadCbo = CreateVBO((int)m_defaultQuad.vertices.size() * 3);
        m_quadCbo.UploadData(ExtractVertexColors(m_defaultQuad.vertices));
        m_quadCbo.CreateVertexAttribPointer(1, 3, GL_FLOAT);

        m_quadUbo = CreateVBO((int)m_defaultQuad.vertices.size() * 3);
        m_quadUbo.UploadData(ExtractVertexTexCoords(m_defaultQuad.vertices));
        m_quadUbo.CreateVertexAttribPointer(2, 2, GL_FLOAT);

        m_quadIbo = CreateIBO((int)m_defaultQuad.indices.size());
        m_quadIbo.UploadData(m_defaultQuad.indices);

        m_quadVao.vbo = std::make_shared<VBO>(m_quadVbo);
        m_quadVao.cbo = std::make_shared<VBO>(m_quadCbo);
        m_quadVao.ubo = std::make_shared<VBO>(m_quadUbo);
        m_quadVao.ibo = std::make_shared<IBO>(m_quadIbo);

        m_quadVao.Unbind();

        //initialize shaders
        m_vertexShader = CreateShader(VERTEX_SHADER);
        m_vertexShader.Load("./assets/shaders/opengl/unlit.vert");
        m_screenSpaceVertexShader = CreateShader(VERTEX_SHADER);
        m_screenSpaceVertexShader.Load("./assets/shaders/opengl/screenSpace.vert");
        m_gridVertexShader = CreateShader(VERTEX_SHADER);
        m_gridVertexShader.Load("./assets/shaders/opengl/grid.vert");
        m_fragmentShader = CreateShader(FRAGMENT_SHADER);
        m_fragmentShader.Load("./assets/shaders/opengl/unlit.frag");
        m_checkeredFragmentShader = CreateShader(FRAGMENT_SHADER);
        m_checkeredFragmentShader.Load("./assets/shaders/opengl/checkers.frag");
        m_gridFragmentShader = CreateShader(FRAGMENT_SHADER);
        m_gridFragmentShader.Load("./assets/shaders/opengl/grid.frag");

        m_shaderProgram = CreateProgram();
        m_shaderProgram.Load(std::shared_ptr<ShaderObject>(&m_vertexShader), std::shared_ptr<ShaderObject>(&m_fragmentShader));
        m_checkeredProgram = CreateProgram();
        m_checkeredProgram.Load(std::shared_ptr<ShaderObject>(&m_screenSpaceVertexShader), std::shared_ptr<ShaderObject>(&m_checkeredFragmentShader));
        m_gridProgram = CreateProgram();
        m_gridProgram.Load(std::shared_ptr<ShaderObject>(&m_gridVertexShader), std::shared_ptr<ShaderObject>(&m_gridFragmentShader));

        //initialize vertex data
        m_vao = CreateVAO();

        m_vbo = CreateVBO((int)scene->models[0].mesh.vertices.size() * 3);
        m_vbo.UploadData(ExtractVertexPositions(scene->models[0].mesh.vertices));
        m_vbo.CreateVertexAttribPointer(0, 3, GL_FLOAT);

        m_cbo = CreateVBO((int)scene->models[0].mesh.vertices.size() * 3);
        m_cbo.UploadData(ExtractVertexColors(scene->models[0].mesh.vertices));
        m_cbo.CreateVertexAttribPointer(1, 3, GL_FLOAT);

        m_ibo = CreateIBO((int)scene->models[0].mesh.indices.size());
        m_ibo.UploadData(scene->models[0].mesh.indices);

        m_vao.vbo = std::make_shared<VBO>(m_vbo);
        m_vao.ibo = std::make_shared<IBO>(m_ibo);

        m_vao.Unbind();
    }

    void OpenGLAPI::BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene> scene, glm::vec2 viewport) {
        //clear outside viewport
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //begin viewport rendering
        m_frameBuffer.Bind();
        m_frameBuffer.Rescale(viewport.x, viewport.y, std::make_shared<Texture>(m_renderTexture));
        glViewport(0, 0, viewport.x, viewport.y);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawCheckeredBackground(viewport);
        DrawGrid(viewport, scene->camera);

        for (Model model : scene->models) {
            //use shaders
            m_shaderProgram.Use();
            m_shaderProgram.UploadUniformMat4("model", model.transform);
            m_shaderProgram.UploadUniformMat4("view", scene->camera.view);
            glm::mat4 projection = glm::perspective(glm::radians(scene->camera.fov), viewport.x / viewport.y, 0.001f, 10000.0f);
            m_shaderProgram.UploadUniformMat4("projection", projection);

            //draw to viewport
            glBindVertexArray(m_vao.id);
            glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //reset
            glUseProgram(0);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_frameBuffer.Unbind();
    }

    void OpenGLAPI::EndDrawFrame() {
        glfwSwapBuffers(p_window->GetGLFWWindow());
    }

    void OpenGLAPI::CleanUp() {
        m_vao.Delete();
        m_vbo.Delete();
        m_cbo.Delete();
        m_ibo.Delete();
        m_frameBuffer.Delete();
        m_renderTexture.Delete();
        m_shaderProgram.CleanUp();
    }

    void OpenGLAPI::UpdateVertexBuffer(std::vector<Vertex> vertices) {
        glDeleteBuffers(1, &m_vbo.id);
        m_vao.Bind();

        //vertices
        m_vbo = CreateVBO((int)vertices.size() * 3);
        m_vbo.UploadData(ExtractVertexPositions(vertices));
        m_vbo.CreateVertexAttribPointer(0, 3, GL_FLOAT);

        m_cbo = CreateVBO((int)vertices.size() * 3);
        m_cbo.UploadData(ExtractVertexColors(vertices));
        m_cbo.CreateVertexAttribPointer(1, 3, GL_FLOAT);

        m_vao.Unbind();
    }

    void OpenGLAPI::UpdateIndexBuffer(std::vector<uint32_t> indices) {
        m_ibo.Delete();
        m_vao.Bind();

        m_ibo = CreateIBO((int)indices.size());
        m_ibo.UploadData(indices);

        m_vao.Unbind();
    }

}
