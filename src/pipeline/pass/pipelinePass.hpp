#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINEPASS_HPP
#define GL_COLOUR_GRADING_PIPELINEPASS_HPP

#include "../../resources/texture.hpp"

namespace GLCG::Pipelines {
    class PipelinePass {
        public:
            PipelinePass() = default;

            virtual void apply();
        private:
            Resources::Texture* texture = nullptr;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINEPASS_HPP
