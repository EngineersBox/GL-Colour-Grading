#include "circularMultiFbo.hpp"

namespace GLCG::Device::GPU::Buffers {
    #define BUFFER_COUNTER_ITERATOR for (int i = 0; i < BUFFER_COUNT; i++)
    CircularMultiFBO::CircularMultiFBO(const int width, const int height):
        BindableResource(),
        width(width),
        height(height) {
        BUFFER_COUNTER_ITERATOR {
            this->textures[i] = Resources::Texture(
                this->width,
                this->height,
                Utils::String::format("FBO Texture %d", i).c_str()
            );
            this->fbos[i] = FBO(this->textures[i]);
        }
    }

    void CircularMultiFBO::bindNextFBO() {
        this->fbos[this->fboIndex].bind();
        this->fboIndex = (this->fboIndex + 1) % 2;
    }

    void CircularMultiFBO::bindNextTexture() {
        this->textures[this->textureIndex].bind();
        this->textureIndex = (this->textureIndex + 1) % 2;
    }

    void CircularMultiFBO::destroy() {
        BUFFER_COUNTER_ITERATOR {
            this->textures[i].destroy();
            this->fbos[i].destroy();
        }
    }
}