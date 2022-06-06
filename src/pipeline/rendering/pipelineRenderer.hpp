#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINERENDERER_HPP
#define GL_COLOUR_GRADING_PIPELINERENDERER_HPP

#include <memory>

#include "../pipeline.hpp"

namespace GLCG::Pipelines {
    class PipelineRenderer {
        public:
            PipelineRenderer() = delete;
            PipelineRenderer(std::unique_ptr<Pipeline> pipeline):
                pipeline(std::move(pipeline)) {};

            virtual void render();
        private:
            std::unique_ptr<Pipeline> pipeline;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINERENDERER_HPP
