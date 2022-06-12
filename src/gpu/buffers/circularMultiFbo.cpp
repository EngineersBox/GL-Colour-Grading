#include "circularMultiFbo.hpp"

namespace GLCG::GPU::Buffers {
    CircularMultiFBO::CircularMultiFBO(const int width, const int height):
        BindableResource(),
        fbos{
            FBO(width, height),
            FBO(width, height)
        }{}

    void CircularMultiFBO::bind() {
        this->fbos[this->index].bind();
        this->index = (this->index + 1) % 2;
    }

    void CircularMultiFBO::destroy() {
        for (FBO fbo : this->fbos) {
            fbo.destroy();
        }
    }
}