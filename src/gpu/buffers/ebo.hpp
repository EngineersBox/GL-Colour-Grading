#pragma once

#ifndef GL_COLOUR_GRADING_EBO_HPP
#define GL_COLOUR_GRADING_EBO_HPP

#include <glad/glad.h>
#include "gpuBuffer.hpp"

namespace GLCG::GPU::Buffers {
    class EBO: public GpuBuffer {
        public:
            GLuint id;

            EBO(GLuint* indices, GLsizeiptr size);

            void bind() override;
            void unbind() override;
            void remove() override;
    };
}

#endif //GL_COLOUR_GRADING_EBO_HPP
