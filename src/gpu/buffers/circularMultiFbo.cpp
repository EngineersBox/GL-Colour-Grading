#include "circularMultiFbo.hpp"

namespace GLCG::GPU::Buffers {
    CircularMultiFBO::CircularMultiFBO(const int width, const int height):
        BindableResource(),
        fbos{
            FBO(width, height),
            FBO(width, height)
        }{}

    void CircularMultiFBO::bindNextFBO() {
        this->fbos[this->fboIndex].bind();
        this->fboIndex = (this->fboIndex + 1) % 2;
    }

    void CircularMultiFBO::bindNextTexture() {
        glBindTexture(GL_TEXTURE_2D, this->fbos[this->textureIndex].getTextureId());
        this->textureIndex = (this->textureIndex + 1) % 2;
    }

    void CircularMultiFBO::destroy() {
        for (FBO fbo : this->fbos) {
            fbo.destroy();
        }
    }
}