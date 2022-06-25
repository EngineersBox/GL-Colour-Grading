#pragma once

#ifndef GL_COLOUR_GRADING_CIRCULAR_MULTI_FBO_HPP
#define GL_COLOUR_GRADING_CIRCULAR_MULTI_FBO_HPP

#include <glad/glad.h>
#include <array>

#include "../bindableResource.hpp"
#include "fbo.hpp"
#include "../../resources/texture.hpp"

namespace GLCG::Device::GPU::Buffers {
    class CircularMultiFBO: public BindableResource {
        public:
            CircularMultiFBO(const int width, const int height);

            void bind() override {
                throw std::runtime_error("Cannot uniformly bind CircularMultiFBO without texture or FBO identification");
            };
            void unbind() override {
                throw std::runtime_error("Cannot unbind CircularMultiFBO");
            };
            void bindNextFBO();
            void bindNextTexture();
            void destroy() override;

            [[nodiscard]]
            constexpr int getWidth() const {
                return this->width;
            }
            [[nodiscard]]
            constexpr int getHeight() const {
                return this->height;
            }
        private:
            #define BUFFER_COUNT 2
            std::array<FBO, BUFFER_COUNT> fbos;
            std::array<Resources::Texture, BUFFER_COUNT> textures;
            unsigned int fboIndex = 0;
            unsigned int textureIndex = 0;

            int width;
            int height;
    };
}

#endif // GL_COLOUR_GRADING_CIRCULAR_MULTI_FBO_HPP
