#include "pipelineRenderer.hpp"

#include <stdexcept>

namespace GLCG:: Pipelines {
    PipelineRenderer::PipelineRenderer(std::unique_ptr<Pipeline> pipeline,
                                       const int width,
                                       const int height):
        pipeline(std::move(pipeline)),
        fbo(GPU::Buffers::CircularMultiFBO(width, height)){}

    void PipelineRenderer::render() {
        for (const CoreVertexMeta& vertex : this->pipeline->getVertexBundleIterator()) {
            switch (vertex.type) {
                case VertexType::NORMAL:
                    invokeWrappedRenderPass([this, &vertex]() -> void {
                        renderNormalPass(dynamic_cast<const NormalVertex&>(vertex));
                    });
                    break;
                case VertexType::BLEND:
                    invokeWrappedRenderPass([this, &vertex]() -> void {
                        renderBlendPass(dynamic_cast<const BlendVertex&>(vertex));
                    });
                    break;
                default: std::runtime_error(Utils::String::format(
                    "Cannot render pipeline pass \"%s\" with no specified type",
                    vertex.name.c_str()
                ));
            }
        }
    }

    void PipelineRenderer::invokeWrappedRenderPass(const typename PipelineRenderer::RenderPassMethod& renderPassMethod) {
        this->preRenderIteration();
        renderPassMethod();
        this->postRenderIteration();
    }

    void PipelineRenderer::renderBlendPass(const BlendVertex& blendVertex) {

    }

    void PipelineRenderer::renderNormalPass(const NormalVertex& normalVertex) {

    }

    void PipelineRenderer::preRenderIteration() {
        this->fbo.bindNextFBO();
    }

    void PipelineRenderer::postRenderIteration() {
        this->fbo.bindNextTexture();
    }
}