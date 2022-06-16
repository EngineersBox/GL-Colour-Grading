#pragma once

#ifndef GL_COLOUR_GRADING_VBO_HPP
#define GL_COLOUR_GRADING_VBO_HPP

#include <glad/glad.h>
#include "bindableResource.hpp"

namespace GLCG::Device::GPU::Buffers {
    class VBO: public BindableResource {
        public:
            GLuint id;

            VBO(const GLfloat* vertices, GLsizeiptr size);
            virtual ~VBO() = default;

            void bind() override;
            void unbind() override;
            void destroy() override;
    };
}

#endif //GL_COLOUR_GRADING_VBO_HPP
