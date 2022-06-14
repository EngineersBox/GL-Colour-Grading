#include "pipelineRenderer.hpp"

#include <stdexcept>

namespace GLCG:: Pipelines {
    PipelineRenderer::PipelineRenderer(std::unique_ptr<Pipeline> pipeline,
                                       const int width,
                                       const int height):
        pipeline(std::move(pipeline)),
        fbo(GPU::Buffers::CircularMultiFBO(width, height)){}

    void PipelineRenderer::render() {
        for (CoreGraph::Vertex& vertex : this->pipeline->getVertexIteratorRange()) {
            CoreVertexMeta& vertexMeta = this->pipeline->getVertex(vertex);
            switch (vertexMeta.type) {
                case VertexType::NORMAL:
                    invokeWrappedRenderPass([this, &vertex, &vertexMeta]() -> void {
                        renderNormalPass(&vertex, dynamic_cast<NormalVertex*>(&vertexMeta));
                    });
                    break;
                case VertexType::BLEND:
                    invokeWrappedRenderPass([this, &vertex, &vertexMeta]() -> void {
                        renderBlendPass(&vertex, dynamic_cast<BlendVertex*>(&vertexMeta));
                    });
                    break;
                default: std::runtime_error(Utils::String::format(
                    "Cannot render pipeline pass \"%s\" with no specified type",
                    vertexMeta.name.c_str()
                ));
            }
        }
    }

    void PipelineRenderer::renderParallel() {

    }

    void PipelineRenderer::invokeWrappedRenderPass(const typename PipelineRenderer::RenderPassMethod& renderPassMethod) {
        this->preRenderIteration();
        renderPassMethod();
        this->postRenderIteration();
    }

    void PipelineRenderer::renderBlendPass(CoreGraph::Vertex* vertex, BlendVertex* blendVertex) const {

    }

    void PipelineRenderer::renderNormalPass(CoreGraph::Vertex* vertex, NormalVertex* normalVertex) const {
        normalVertex->pass.apply();
    }

    void PipelineRenderer::preRenderIteration() {
        this->fbo.bindNextFBO();
    }

    void PipelineRenderer::postRenderIteration() {
        this->fbo.bindNextTexture();
    }
}