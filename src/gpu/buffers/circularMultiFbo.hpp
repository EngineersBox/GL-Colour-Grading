#pragma once

#ifndef GL_COLOUR_GRADING_CIRCULARMULTITEXTUREFBO_HPP
#define GL_COLOUR_GRADING_CIRCULARMULTITEXTUREFBO_HPP

#include <glad/glad.h>
#include "../bindableResource.hpp"
#include "fbo.hpp"

namespace GLCG::GPU::Buffers {
    class CircularMultiFBO: public BindableResource {
        public:
            CircularMultiFBO(const int width, const int height);

            void bind() override;
            void destroy() override;
        private:
            FBO fbos[2];
            unsigned int index = 0;
    };
}

#endif //GL_COLOUR_GRADING_CIRCULARMULTITEXTUREFBO_HPP
