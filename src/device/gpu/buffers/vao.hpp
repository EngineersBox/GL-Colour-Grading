#pragma once

#ifndef GL_COLOUR_GRADING_VAO_HPP
#define GL_COLOUR_GRADING_VAO_HPP

#include <glad/glad.h>
#include "vbo.hpp"
#include "bindableResource.hpp"

namespace GLCG::Device::GPU::Buffers {
    class VAO: public BindableResource {
        public:
            GLuint id;

            VAO(const VAO&) = default;
            VAO(VAO&&) = default;
            VAO();
            virtual ~VAO() = default;

            static void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
            void bind() override;
            void unbind() override;
            void destroy() override;
    };
}

#endif //GL_COLOUR_GRADING_VAO_HPP
