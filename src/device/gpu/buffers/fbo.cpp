#include "fbo.hpp"

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace GLCG::Device::GPU::Buffers {
    FBO::FBO(Resources::Texture& texture) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        this->attachTexture(texture);
        if (const GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error(Utils::String::format(
                "Error during framebuffer initialisation [%zu]: %s",
                fboStatus
            ));
        }
    }

    void FBO::activate() const {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void FBO::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void FBO::finalise() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::destroy() {
        glDeleteFramebuffers(1, &this->fbo);
    }

    void FBO::attachTexture(Resources::Texture& texture) {
        this->bind();
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            texture.getId(),
            0
        );
    }
}