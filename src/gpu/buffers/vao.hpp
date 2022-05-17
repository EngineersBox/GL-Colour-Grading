#pragma once

#ifndef GL_COLOUR_GRADING_VAO_HPP
#define GL_COLOUR_GRADING_VAO_HPP

#include <glad/glad.h>
#include "vbo.hpp"

namespace GLCG::GPU::Buffers {
    class VAO {
        public:
            GLuint id;

            VAO();

            void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
            void bind();
            void unbind();
            void remove();
    };
}

#endif //GL_COLOUR_GRADING_VAO_HPP
