#pragma once

#ifndef GL_COLOUR_GRADING_EBO_HPP
#define GL_COLOUR_GRADING_EBO_HPP

#include <glad/glad.h>
#include "bindableResource.hpp"

namespace GLCG::GPU::Buffers {
    class EBO: public BindableResource {
        public:
            GLuint id;

            EBO(GLuint* indices, GLsizeiptr size);

            void bind() override;
            void unbind() override;
            void destroy() override;
    };
}

#endif //GL_COLOUR_GRADING_EBO_HPP
