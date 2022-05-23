#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINEPASS_HPP
#define GL_COLOUR_GRADING_PIPELINEPASS_HPP

#include "../../gpu/shaders/shader.hpp"
#include "../../resources/texture.hpp"

namespace GLCG::Pipelines {
    class PipelinePass {
        public:
            PipelinePass() = default;

            virtual void apply();
        private:
            GPU::Shaders::Shader* shader = nullptr;
            Resources::Texture* texture = nullptr;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINEPASS_HPP
