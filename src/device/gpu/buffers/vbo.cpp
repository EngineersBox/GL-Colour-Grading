#include "vbo.hpp"

namespace GLCG::Device::GPU::Buffers {
    VBO::VBO(const GLfloat* vertices, GLsizeiptr size) {
        glGenBuffers(1, &this->id);
        glBindBuffer(GL_ARRAY_BUFFER, this->id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void VBO::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, this->id);
    }

    void VBO::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBO::destroy() {
        glDeleteBuffers(1, &this->id);
    }
}