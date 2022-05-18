#include "fbo.hpp"

#include <spdlog/spdlog.h>

namespace GLCG::GPU::Buffers {
    FBO::FBO(const int width, const int height):
        shader(
            "../assets/shaders/framebuffer.vsh",
            "../assets/shaders/framebuffer.fsh"
        ) {
        spdlog::info("Compiled and linked core framebuffer");
        activate();
        glUniform1i(glGetUniformLocation(this->shader.id, "screenTexture"), 0);

        glEnable(GL_DEPTH_TEST);

        // Prepare frame buffer rectangle VBO and VAO
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(FBO::rectangleVertices), &FBO::rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Create frame buffer object
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create frame buffer Texture
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Create render buffer object
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            spdlog::error("Error during framebuffer intialisation: {}", fboStatus);
            exit(1);
        }
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
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void FBO::destroy() {
        glDeleteFramebuffers(1, &this->fbo);
        this->shader.destroy();
    }
}