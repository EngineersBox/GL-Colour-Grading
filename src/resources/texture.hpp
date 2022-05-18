#pragma once

#ifndef GL_COLOUR_GRADING_TEXTURE_HPP
#define GL_COLOUR_GRADING_TEXTURE_HPP

#include <glad/glad.h>

#include "../gpu/bindableResource.hpp"
#include "../gpu/shaders/shader.hpp"

namespace GLCG::Resources {
    class Texture: public GPU::BindableResource {
        public:
            GLuint id;
            GLenum type;
            Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

            void assignTextureUnit(GPU::Shaders::Shader& shader, const char* uniform, GLuint unit);
            void bind() override;
            void unbind() override;
            void destroy() override;
    };
}

#endif //GL_COLOUR_GRADING_TEXTURE_HPP
