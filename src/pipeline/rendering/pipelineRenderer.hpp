#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINERENDERER_HPP
#define GL_COLOUR_GRADING_PIPELINERENDERER_HPP

#include <memory>
#include <functional>

#include "../pipeline.hpp"
#include "../../gpu/buffers/circularMultiFbo.hpp"

namespace GLCG::Pipelines {
    class PipelineRenderer {
        public:
            PipelineRenderer() = delete;
            PipelineRenderer(std::unique_ptr<Pipeline> pipeline,
                             int width,
                             int height);

            virtual void render();
            virtual void renderParallel();
            virtual void renderBlendPass(const BlendVertex& blendVertex);
            virtual void renderNormalPass(const NormalVertex& normalVertex);

            using RenderPassMethod = std::function<void(void)>;
            void invokeWrappedRenderPass(const RenderPassMethod& renderPassMethod);
        private:
            std::unique_ptr<Pipeline> pipeline;
            GPU::Buffers::CircularMultiFBO fbo;

            void preRenderIteration();
            void postRenderIteration();
    };
}

#endif //GL_COLOUR_GRADING_PIPELINERENDERER_HPP
