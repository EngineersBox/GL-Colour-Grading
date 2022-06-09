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

    struct NormalVertex: public CoreVertexMeta {
        explicit NormalVertex(std::string const& name, PipelinePass pass):
            CoreVertexMeta(name, VertexType::NORMAL),
            pass(std::move(pass)) {}
        PipelinePass pass;
    };

    struct BlendVertex: public CoreVertexMeta {
        explicit BlendVertex(std::string const& name, PipelinePassBlendMode blendMode):
            CoreVertexMeta(name, VertexType::BLEND),
            blendMode(blendMode) {}
        PipelinePassBlendMode blendMode;
        virtual std::string toString() override;
    };

    class Pipeline {
        public:
            Vertex addVertex(const CoreVertexMeta& vertex);
            std::pair<Edge, bool> addEdge(Vertex vertex1, Vertex vertex2);
            void removeVertex(const VertexIterator& vertexIterator);
            void removeVertex(const std::string& name);

            [[nodiscard]]
            boost::iterator_range<VertexIterator> getVertexIteratorRange() noexcept;
            [[nodiscard]]
            boost::iterator_range<VertexIterator> getVertexIteratorRange() const noexcept;

            [[nodiscard]]
            boost::iterator_range<EdgeIterator> getEdgeIteratorRange() noexcept;
            [[nodiscard]]
            boost::iterator_range<EdgeIterator> getEdgeIteratorRange() const noexcept;

            [[nodiscard]]
            CoreGraph::VertexBundleIterator getVertexBundleIterator() noexcept;
//            [[nodiscard]]
//            CoreGraph::VertexBundleConstIterator getVertexBundleIterator() const noexcept;

            [[nodiscard]]
            std::string graphToString();
        private:
            CoreGraph graph;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINE_HPP
