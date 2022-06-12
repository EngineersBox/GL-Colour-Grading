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
                    preRenderIteration();
                    renderNormalPass(dynamic_cast<const NormalVertex&>(vertex));
                    postRenderIteration();
                    break;
                case VertexType::BLEND:
                    preRenderIteration();
                    renderBlendPass(dynamic_cast<const BlendVertex&>(vertex));
                    postRenderIteration();
                    break;
                default: std::runtime_error(Utils::String::format(
                    "Cannot render pipeline pass \"%s\" with no specified type",
                    vertex.name.c_str()
                ));
            }
        }
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