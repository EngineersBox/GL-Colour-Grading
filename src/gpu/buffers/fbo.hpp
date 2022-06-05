#pragma once

#ifndef GL_COLOUR_GRADING_FBO_HPP
#define GL_COLOUR_GRADING_FBO_HPP

#include <array>
#include <stdexcept>

#include "../shaders/shader.hpp"
#include "../bindableResource.hpp"

namespace GLCG::GPU::Buffers {
    class FBO: public BindableResource {
        public:
            FBO(int width, int height);
            virtual ~FBO() = default;

            void activate() const;
            void finalise() const;
            void bind() override;
            void unbind() override {
                throw std::runtime_error("Cannot unbind FBO");
            };
            void destroy() override;

            [[nodiscard]]
            constexpr unsigned int getTextureId() const noexcept {
                return this->framebufferTexture;
            }
        private:
            GLCG::GPU::Shaders::Shader shader;
            unsigned int rectVAO = 0;
            unsigned int rectVBO = 0;
            unsigned int fbo = 0;
            unsigned int framebufferTexture = 0;
            unsigned int rbo = 0;

            static constexpr std::array<float, 24> const RECTANGLE_VERTICES = {
                1.0f, -1.0f,  1.0f, 0.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 1.0f,
                1.0f,  1.0f,  1.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f,  0.0f, 1.0f
            };
            static constexpr const char* FRAMEBUFFER_FRAGMENT_SHADER_PATH = "../assets/shaders/framebuffer.fsh";
            static constexpr const char* FRAMEBUFFER_VERTEX_SHADER_PATH = "../assets/shaders/framebuffer.vsh";

            void prepareRectangleVBOVAO();
            void createFBO();
            void createFBOTexture(int width, int height);
            void createRBO(int width, int height);
    };
}

#endif //GL_COLOUR_GRADING_FBO_HPP
