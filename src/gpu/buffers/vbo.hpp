#pragma once

#ifndef GL_COLOUR_GRADING_VBO_HPP
#define GL_COLOUR_GRADING_VBO_HPP

#include <glad/glad.h>
#include "bindableResource.hpp"

namespace GLCG::GPU::Buffers {
    class VBO: public BindableResource {
        public:
            GLuint id;

            VBO(GLfloat* vertices, GLsizeiptr size);

            void bind() override;
            void unbind() override;
            void destroy() override;
    };
}

#endif //GL_COLOUR_GRADING_VBO_HPP
