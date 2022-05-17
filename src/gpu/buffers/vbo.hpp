#pragma once

#ifndef GL_COLOUR_GRADING_VBO_HPP
#define GL_COLOUR_GRADING_VBO_HPP

#include <glad/glad.h>

namespace GLCG::GPU::Buffers {
    class VBO {
        public:
            GLuint id;

            VBO(GLfloat* vertices, GLsizeiptr size);

            void bind();
            void unbind();
            void remove();
    };
}

#endif //GL_COLOUR_GRADING_VBO_HPP
