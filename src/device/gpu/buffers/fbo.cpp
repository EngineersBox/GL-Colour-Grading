#include "fbo.hpp"

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace GLCG::Device::GPU::Buffers {
    FBO::FBO(const int width, const int height) {
        this->shader = Shaders::Shader::builder()
            .withVertex(FBO::FRAMEBUFFER_VERTEX_SHADER_PATH)
            .withFragment(FBO::FRAMEBUFFER_FRAGMENT_SHADER_PATH);
        spdlog::info("Compiled and linked framebuffer shader");
        this->shader.activate();
        glUniform1i(glGetUniformLocation(this->shader.id, "screenTexture"), 0);

        glEnable(GL_DEPTH_TEST);

        prepareRectangleVBOVAO();
        createFBO();
        createFBOTexture(width, height);
        createRBO(width, height);
        this->shader.validateProgram();

        if (const GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error(Utils::String::format(
                "Error during framebuffer initialisation: %zu",
                fboStatus
            ));
        }
    }

    void FBO::prepareRectangleVBOVAO() {
        glGenVertexArrays(1, &this->rectVAO);
        glGenBuffers(1, &this->rectVBO);
        glBindVertexArray(this->rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(FBO::RECTANGLE_VERTICES), &FBO::RECTANGLE_VERTICES, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    void FBO::createFBO() {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void FBO::createFBOTexture(const int width, const int height) {
        glGenTextures(1, &this->framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, this->framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->framebufferTexture, 0);
    }

    void FBO::createRBO(const int width, const int height) {
        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    }

    void FBO::activate() const {
        this->shader.activate();
    }

    void FBO::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void FBO::finalise() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        this->shader.activate();
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, this->framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void FBO::destroy() {
        glDeleteFramebuffers(1, &this->fbo);
        this->shader.destroy();
    }
}