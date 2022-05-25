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

    static std::string blendModeToString(PipelinePassBlendMode blendMode) {
        switch (blendMode) {
            case PipelinePassBlendMode::ADDITIVE: return "ADDITIVE";
            case PipelinePassBlendMode::SUBTRACTIVE: return "SUBTRACTIVE";
            case PipelinePassBlendMode::LIGHTEN: return "LIGHTEN";
            case PipelinePassBlendMode::DARKEN: return "DARKEN";
            default: throw std::invalid_argument("Unknown blend mode");
        }
    }

    struct NormalVertex: public CoreVertexMeta {
        PipelinePass pass;
    };

    struct BlendVertex: public CoreVertexMeta {
        PipelinePassBlendMode blendMode;
        virtual std::string toString() override;
    };

    class Pipeline {
        public:
            Pipeline() = default;

            size_t addPass(const std::string& name, PipelinePass const& pass);
            size_t addParallelPass(const std::string& name, CoreGraph& parallelPasses, Vertex& toAppendAfter, Vertex& toAppendFrom);
            size_t addBlendPass(const std::string& name, PipelinePass const& blendPass, PipelinePassBlendMode blendMode);
            void removePass();

            [[nodiscard]]
            std::string graphToString();

            [[nodiscard]]
            boost::iterator_range<CoreGraph::vertex_iterator> getVertexIteratorRange() noexcept;
            [[nodiscard]]
            boost::iterator_range<CoreGraph::vertex_iterator> getVertexIteratorRange() const noexcept;

            [[nodiscard]]
            boost::iterator_range<CoreGraph::edge_iterator> getEdgeIteratorRange() noexcept;
            [[nodiscard]]
            boost::iterator_range<CoreGraph::edge_iterator> getEdgeIteratorRange() const noexcept;

        private:
            CoreGraph graph;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINE_HPP
