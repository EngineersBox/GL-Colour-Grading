#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINE_HPP
#define GL_COLOUR_GRADING_PIPELINE_HPP

#include <utility>
#include <algorithm>
#include <memory>
#include <map>
#include <sstream>
#include <stdexcept>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>

#include "pipelinePass.hpp"
#include "graph.hpp"
#include "../util/stringUtils.hpp"

namespace GLCG::Pipelines {
    enum class PipelinePassBlendMode {
        ADDITIVE,
        SUBTRACTIVE,
        LIGHTEN,
        DARKEN
    };

    class Pipeline {
        public:
            Pipeline() = default;

            size_t addPass(PipelinePass const& pass) {
//                this->graph.emplace_back(NormalNode<PipelinePass>(pass));
                return this->graph.num_vertices();
            }

            size_t addParallelPass(CoreGraph& parallelPasses, Vertex& toAppendAfter, Vertex& toAppendFrom) {
//                if (this->graph.empty()) {
//                    throw std::runtime_error("Parallel pass cannot be first node");
//                } else if (this->graph.back().type == NodeType::PARALLEL) {
//                    throw std::runtime_error("Cannot chain non-nested parallel passes without intermediary join");
//                }
//                this->graph.emplace_back(ParallelNode<PipelinePass>(parallelPasses));

                mergeGraphs(this->graph, toAppendAfter, parallelPasses, toAppendFrom);
                return this->graph.num_vertices();
            }

            size_t addBlendPass(PipelinePass const& blendPass, PipelinePassBlendMode blendMode) {
//                if (this->graph.empty()) {
//                    throw std::runtime_error("Parallel pass cannot be first node");
//                } else if (this->graph.back().type != NodeType::PARALLEL) {
//                    throw std::runtime_error("Cannot join from a non-parallel pass");
//                }
//                this->graph.emplace_back(JoinNode<PipelinePass, PipelinePassBlendMode>(blendPass, blendMode));
                return this->graph.num_vertices();
            }

            void removePass() {

            }

            [[nodiscard]]
            std::string graphToString() {
                std::stringstream ss;
                auto bundleMap = get(boost::vertex_bundle, this->graph);
                for (Vertex v : boost::make_iterator_range(vertices(this->graph))) {
                    CoreVertexMeta& bundle = bundleMap[v];
                    ss << bundle.toString();
                }
                return ss.str();
            }

        private:
            CoreGraph graph;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINE_HPP
