#include "vbo.hpp"

namespace GLCG::GPU::Buffers {
    VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
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

    void VBO::remove() {
        glDeleteBuffers(1, &this->id);
    }
}