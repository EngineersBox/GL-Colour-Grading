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
            const char* type;
            GLuint unit;

            Texture(const char* image, const char* texType, GLuint slot);

            static void assignTextureUnit(GPU::Shaders::Shader& shader, const char* uniform, GLuint unit);
            void bind() override;
            void unbind() override;
            void destroy() override;

            [[nodiscard]] constexpr int getWidth() const noexcept {
                return this->width;
            }
            [[nodiscard]] constexpr int getHeight() const noexcept {
                return this->height;
            }
            [[nodiscard]] constexpr int getChannels() const noexcept {
                return this->channels;
            }
        private:
            int width = 0;
            int height = 0;
            int channels = 0;

            inline void generateTexImage(uint8_t const* bytes) const;
    };
}

#endif //GL_COLOUR_GRADING_TEXTURE_HPP
