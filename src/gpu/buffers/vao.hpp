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

            void linkVBO(VBO& VBO, GLuint layout);
            void bind();
            void unbind();
            void remove();
    };
}

#endif //GL_COLOUR_GRADING_VAO_HPP
