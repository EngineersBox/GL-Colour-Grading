#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINE_HPP
#define GL_COLOUR_GRADING_PIPELINE_HPP

#include <utility>
#include <algorithm>
#include <memory>
#include <map>
#include <sstream>
#include <stdexcept>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>

#include "pipelinePass.hpp"
#include "core/graph.hpp"
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

    struct SerialVertex: public CoreVertexMeta {
        explicit SerialVertex(std::string const& name,
                              PipelinePass const& pass):
            CoreVertexMeta(name, VertexType::SERIAL),
            pass(pass) {}
        PipelinePass pass;
    };

    struct ParallelMixerVertex: public CoreVertexMeta {
        explicit ParallelMixerVertex(std::string const& name,
                                     PipelinePass const& pass,
                                     PipelinePassBlendMode blendMode):
            CoreVertexMeta(name, VertexType::PARALLEL_MIXER),
            pass(pass),
            blendMode(blendMode) {}
            PipelinePass pass;
        PipelinePassBlendMode blendMode;
        virtual std::string toString() override;
    };

    struct LayerMixerVertex: public CoreVertexMeta {
        explicit LayerMixerVertex(std::string const& name,
                                     PipelinePass const& pass,
                                     PipelinePassBlendMode blendMode):
            CoreVertexMeta(name, VertexType::LAYER_MIXER),
            pass(pass),
            blendMode(blendMode) {}
        PipelinePass pass;
        PipelinePassBlendMode blendMode;
        virtual std::string toString() override;
    };

    class Pipeline {
        public:
            CoreGraph::Vertex addVertex(const CoreVertexMeta& vertex);
            std::pair<CoreGraph::Edge, bool> addEdge(CoreGraph::Vertex vertex1, CoreGraph::Vertex vertex2);
            void removeVertex(const CoreGraph::VertexIterator& vertexIterator);
            void removeVertex(const std::string& name);
            CoreVertexMeta& getVertex(CoreGraph::Vertex vertex);

            using VertexIteratorRange = boost::iterator_range<CoreGraph::VertexIterator>;
            using EdgeIteratorRange = boost::iterator_range<CoreGraph::EdgeIterator>;

            [[nodiscard]]
            VertexIteratorRange getVertexIteratorRange() noexcept;
            [[nodiscard]]
            VertexIteratorRange getVertexIteratorRange() const noexcept;

            [[nodiscard]]
            EdgeIteratorRange getEdgeIteratorRange() noexcept;
            [[nodiscard]]
            EdgeIteratorRange getEdgeIteratorRange() const noexcept;

            [[nodiscard]]
            CoreGraph::VertexBundleIterator getVertexBundleIterator() noexcept;

            [[nodiscard]]
            CoreGraph::VertexBundleIterator getNeighbouringInVertexBundleIterator(CoreGraph::Vertex vertex) noexcept;
            [[nodiscard]]
            CoreGraph::VertexBundleIterator getNeighbouringOutVertexBundleIterator(CoreGraph::Vertex vertex) noexcept;

            [[nodiscard]]
            std::string graphToString();
        private:
            CoreGraph graph;
            friend class PipelineRenderer;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINE_HPP
