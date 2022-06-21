#include "pipelineRenderer.hpp"

#include <stdexcept>
#include <utility>
#include <spdlog/spdlog.h>

namespace GLCG:: Pipelines {
    PipelineRenderer::PipelineRenderer(std::unique_ptr<Pipeline> pipeline,
                                       const int width,
                                       const int height):
        pipeline(std::move(pipeline)),
        fbo(Device::GPU::Buffers::CircularMultiFBO(width, height)){}

    void PipelineRenderer::destroy() {
        this->fbo.destroy();
        this->pipeline.release();
    }

    void PipelineRenderer::render() {
        for (CoreGraph::Vertex& vertex : this->pipeline->getVertexIteratorRange()) {
            CoreVertexMeta& vertexMeta = this->pipeline->getVertex(vertex);
            if (vertexMeta.type == VertexType::NONE) {
                throw std::runtime_error(Utils::String::format(
                    "Cannot render pipeline pass \"%s\" with no specified type",
                    vertexMeta.name.c_str()
                ));
            } else if (!this->handlers.contains(vertexMeta.type)) {
                throw std::runtime_error(Utils::String::format(
                    "No handler registered for vertex type %s",
                    vertexTypeToString(vertexMeta.type).c_str()
                ));
            }
            preRenderIteration();
            try {
                this->handlers[vertexMeta.type](
                    &this->pipeline->graph,
                    &vertex,
                    &vertexMeta,
                    this->fbo.getWidth(),
                    this->fbo.getHeight()
                );
            } catch (const std::bad_function_call& e) {
                spdlog::error("Unable to invoke pass render handler: {}", e.what());
                exit(1);
            }
            postRenderIteration();
        }
    }

    void PipelineRenderer::renderParallel() {
        // TODO: implement this
    }

    void PipelineRenderer::preRenderIteration() {
        this->fbo.bindNextFBO();
    }

    void PipelineRenderer::postRenderIteration() {
        this->fbo.bindNextTexture();
    }

//    template<VertexType V>
//    void PipelineRenderer::registerHandler(PassHandler&& handler) {
//        if (this->handlers.contains(V)) {
//            throw std::runtime_error(Utils::String::format(
//                "Handler already registered for vertex type %s",
//                vertexTypeToString(V).c_str()
//            ));
//        }
//        this->handlers[V] = std::move(handler);
//    }
}