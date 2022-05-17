#include "ebo.hpp"

namespace GLCG::GPU::Buffers {
    EBO::EBO(GLuint* indices, GLsizeiptr size) {
        glGenBuffers(1, &this->id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    }

    void EBO::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    }

    void EBO::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void EBO::remove() {
        glDeleteBuffers(1, &this->id);
    }
}