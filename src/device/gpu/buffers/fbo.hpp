#pragma once

#ifndef GL_COLOUR_GRADING_FBO_HPP
#define GL_COLOUR_GRADING_FBO_HPP

#include <array>
#include <stdexcept>

#include "../resources/texture.hpp"
#include "../bindableResource.hpp"

namespace GLCG::Device::GPU::Buffers {
    class FBO: public BindableResource {
        public:
            FBO() = default;
            FBO(Resources::Texture& texture);
            virtual ~FBO() = default;

            virtual void activate() const;
            virtual void finalise() const;
            void bind() override;
            void unbind() override {
                throw std::runtime_error("Cannot unbind FBO");
            };
            void destroy() override;

            void attachTexture(Resources::Texture& texture);
        private:
            unsigned int fbo = 0;
    };
}

#endif //GL_COLOUR_GRADING_FBO_HPP
