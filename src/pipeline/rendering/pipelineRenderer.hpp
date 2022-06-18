#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINERENDERER_HPP
#define GL_COLOUR_GRADING_PIPELINERENDERER_HPP

#include <memory>
#include <functional>
#include <map>

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

            using PassHandler = std::function<void(CoreGraph*, CoreGraph::Vertex*, CoreVertexMeta*)>;

            template<VertexType V>
            void registerHandler(PassHandler&& handler);
        private:
            std::unordered_map<VertexType, PassHandler> handlers;

            std::unique_ptr<Pipeline> pipeline;
            Device::GPU::Buffers::CircularMultiFBO fbo;

            void preRenderIteration();
            void postRenderIteration();
    };

    struct ParallelMixerPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph, CoreGraph::Vertex* vertex, CoreVertexMeta* parallelMixerVertex) {
            if (!graph->hasInVertices(*vertex)) {
                throw std::runtime_error("Parallel mixer node does not have incoming edges, cannot blend connected nodes");
            }
            // TODO: Implement this
        }
    };

    struct LayerMixerPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph, CoreGraph::Vertex* vertex, CoreVertexMeta* layerMixerVertex) {
            if (!graph->hasInVertices(*vertex)) {
                throw std::runtime_error("Layer mixer node does not have incoming edges, cannot blend connected nodes");
            }
            // TODO: Implement this
        }
    };

    struct SerialPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph, CoreGraph::Vertex* vertex, CoreVertexMeta* serialVertex) {
            dynamic_cast<SerialVertex*>(serialVertex)->pass.apply();
        }
    };
}

#endif //GL_COLOUR_GRADING_PIPELINERENDERER_HPP
