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
            void destroy();

            virtual void render();
            virtual void renderParallel();


            /**
             * @Params
             *  CoreGraph*: Graph instance\n
             *  Vertex: Vertex pointer\n
             *  CoreVertexMeta* Vertex descriptor pointer\n
             *  const int: FBO width\n
             *  const int:  FBO height\n
             */
             struct PassHandler {
                 virtual void operator()(CoreGraph*, CoreGraph::Vertex*, CoreVertexMeta*, int, int) {};
             };

            template<VertexType V>
            void registerHandler(PassHandler&& handler) {
                if (this->handlers.contains(V)) {
                    throw std::runtime_error(Utils::String::format(
                        "Handler already registered for vertex type %s",
                        vertexTypeToString(V).c_str()
                    ));
                }
                this->handlers[V] = std::move(handler);
            }
        private:
            std::unordered_map<VertexType, PassHandler> handlers;

            std::unique_ptr<Pipeline> pipeline;
            Device::GPU::Buffers::CircularMultiFBO fbo;

            void preRenderIteration();
            void postRenderIteration();
    };

    struct ParallelMixerPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph,
                        CoreGraph::Vertex* vertex,
                        CoreVertexMeta* parallelMixerVertex,
                        const int fboWidth,
                        const int fboHeight) override {
            if (!graph->hasInVertices(*vertex)) {
                throw std::runtime_error("Parallel mixer node does not have incoming edges, cannot blend connected nodes");
            }
            // TODO: Implement this
        }
    };

    struct LayerMixerPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph,
                        CoreGraph::Vertex* vertex,
                        CoreVertexMeta* layerMixerVertex,
                        const int fboWidth,
                        const int fboHeight) override {
            if (!graph->hasInVertices(*vertex)) {
                throw std::runtime_error("Layer mixer node does not have incoming edges, cannot blend connected nodes");
            }
            // TODO: Implement this
        }
    };

    struct SerialPassHandler: public PipelineRenderer::PassHandler {
        void operator()(CoreGraph* graph,
                        CoreGraph::Vertex* vertex,
                        CoreVertexMeta* serialVertex,
                        const int fboWidth,
                        const int fboHeight) override {
            dynamic_cast<SerialVertex*>(serialVertex)->pass(fboWidth, fboHeight);
        }
    };
}

#endif //GL_COLOUR_GRADING_PIPELINERENDERER_HPP
