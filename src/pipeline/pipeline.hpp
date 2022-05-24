#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINE_HPP
#define GL_COLOUR_GRADING_PIPELINE_HPP

#include <utility>
#include <algorithm>

#include "pipelinePass.hpp"
#include "graph/graph.hpp"
#include "../util/stringUtils.hpp"

namespace GLCG::Pipelines {
    enum PipelinePassBlendMode {};
    class Pipeline {
        public:
            Pipeline() = default;

            size_t addPass(PipelinePass pass) {
                this->graph.emplace_back(std::move(pass));
                return this->graph.size() - 1;
            }
            size_t addParallelPass(std::vector<Graph<PipelinePass>> parallelPasses) {
                if (this->graph.empty()) {
                    throw std::runtime_error("Parallel pass cannot be first node");
                } else if (this->graph.back().type == GraphNodeType::PARALLEL) {
                    throw std::runtime_error("Cannot chain non-nested parallel passes without intermediary join");
                }
                this->graph.emplace_back(std::move(parallelPasses));
                return this->graph.size() - 1;
            }
            size_t addBlendPass(PipelinePass blendPass, PipelinePassBlendMode blendMode) {
                if (this->graph.empty()) {
                    throw std::runtime_error("Parallel pass cannot be first node");
                } else if (this->graph.back().type != GraphNodeType::PARALLEL) {
                    throw std::runtime_error("Cannot join from a non-parallel pass");
                }
                this->graph.emplace_back(std::move(blendPass), blendMode);
                return this->graph.size() - 1;
            }
            void removePass(const size_t index) {
                if (this->graph.empty() || this->graph.size() - 1 > index) {
                    throw std::runtime_error(Utils::String::format(
                        "Pass index out of bounds: {} > {}",
                        std::max((int) this->graph.size() - 1, 0),
                        index
                    ));
                }
                this->graph.erase(std::next(this->graph.begin(), index));
            }

        private:
            Graph<PipelinePass> graph;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINE_HPP
