#pragma once

#ifndef GL_COLOUR_GRADING_VBO_HPP
#define GL_COLOUR_GRADING_VBO_HPP

#include <glad/glad.h>
#include "gpuBuffer.hpp"

namespace GLCG::GPU::Buffers {
    class VBO: public GpuBuffer {
        public:
            GLuint id;

            VBO(GLfloat* vertices, GLsizeiptr size);

            void bind() override;
            void unbind() override;
            void remove() override;
    };
}

#endif //GL_COLOUR_GRADING_VBO_HPP
