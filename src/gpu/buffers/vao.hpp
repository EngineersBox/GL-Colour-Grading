#pragma once

#ifndef GL_COLOUR_GRADING_VAO_HPP
#define GL_COLOUR_GRADING_VAO_HPP

#include <glad/glad.h>
#include "vbo.hpp"
#include "gpuBuffer.hpp"

namespace GLCG::GPU::Buffers {
    class VAO: public GpuBuffer {
        public:
            GLuint id;

            VAO();

            void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
            void bind() override;
            void unbind() override;
            void remove() override;
    };
}

#endif //GL_COLOUR_GRADING_VAO_HPP
