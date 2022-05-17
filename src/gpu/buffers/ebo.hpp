#pragma once

#ifndef GL_COLOUR_GRADING_EBO_HPP
#define GL_COLOUR_GRADING_EBO_HPP

#include <glad/glad.h>

namespace GLCG::GPU::Buffers {
    class EBO {
        public:
            GLuint id;

            EBO(GLuint* indices, GLsizeiptr size);

            void bind();
            void unbind();
            void remove();
    };
}

#endif //GL_COLOUR_GRADING_EBO_HPP
