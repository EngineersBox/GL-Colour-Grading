#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <functional>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace GLCG::Pipelines {
    enum class VertexType {
        NONE,
        NORMAL,
        BLEND
    };

    static std::string vertexTypeToString(VertexType type) {
        switch (type) {
            case VertexType::NORMAL: return "NORMAL";
            case VertexType::BLEND: return "BLEND";
            default: throw std::invalid_argument("Unknown vertex type");
        }
    }

    struct CoreVertexMeta {
        CoreVertexMeta() = default;
        explicit CoreVertexMeta(std::string name, VertexType type):
            name(std::move(name)),
            type(type) {}
        std::string name;
        VertexType type = VertexType::NONE;
        virtual std::string toString();
    };

    template<typename T>
    using InternalCoreGraph = boost::directed_graph<T, boost::no_property, boost::no_property>;

    template<typename T>
    class DirectedGraphWrapper: public InternalCoreGraph<T> {
        private:
            using InternalVertex = typename InternalCoreGraph<T>::vertex_descriptor;
            using InternalVertexIterator = typename InternalCoreGraph<T>::vertex_iterator;
            using InternalEdge = typename InternalCoreGraph<T>::edge_descriptor;

            template<typename R>
            using VertexAccessor = std::function<R(const InternalVertex)>;

            template<typename R>
            using InternalVertexBundleIterator = boost::range_detail::transformed_range<VertexAccessor<R>, const std::pair<InternalVertexIterator, InternalVertexIterator>>;

            template<typename R>
            [[nodiscard]]
            VertexAccessor<R> generateAccessor() noexcept {
                return [map = get(boost::vertex_bundle, *this)](const InternalVertex v) -> R {
                    return map[v];
                };
            }

        public:
            DirectedGraphWrapper() = default;

            using VertexBundleIterator = InternalVertexBundleIterator<T&>;
            using Vertex = typename InternalCoreGraph<T>::vertex_descriptor;
            using VertexIterator = typename InternalCoreGraph<T>::vertex_iterator;
            using Edge = typename InternalCoreGraph<T>::edge_descriptor;
            using EdgeIterator = typename InternalCoreGraph<T>::edge_iterator;

            [[nodiscard]]
            VertexBundleIterator vertexBundlesIterator() noexcept {
                return boost::vertices(*this) | boost::adaptors::transformed(generateAccessor<T&>());
            }

            [[nodiscard]]
            VertexBundleIterator neighbouringVertexBundlesIterator(const InternalVertex vertex) noexcept {
                return boost::adjacent_vertices(vertex, *this) | boost::adaptors::transformed(generateAccessor<T&>());
            }

            InternalVertex firstVertex() noexcept;
            InternalVertex lastVertex() noexcept;
            VertexIterator findVertex(const std::string_view& name);
            bool hasVertex(const std::string_view& name);
            void mergeGraphs(Vertex graph1Vertex,
                             const InternalCoreGraph<CoreVertexMeta>& graph2,
                             Vertex graph2Vertex);
    };

    using CoreGraph = DirectedGraphWrapper<CoreVertexMeta>;
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
