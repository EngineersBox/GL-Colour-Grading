#include "vao.hpp"

namespace GLCG::GPU::Buffers {
    VAO::VAO() {
        glGenVertexArrays(1, &this->id);
    }

    void VAO::linkVBO(VBO& VBO, GLuint layout) {
        VBO.bind();
        glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(layout);
        VBO.unbind();
    }

    void VAO::bind() {
        glBindVertexArray(this->id);
    }

    void VAO::unbind() {
        glBindVertexArray(0);
    }

    void VAO::remove() {
        glDeleteVertexArrays(1, &this->id);
    }
}