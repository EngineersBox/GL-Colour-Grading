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
        SERIAL,
        PARALLEL_MIXER,
        LAYER_MIXER
    };

    static std::string vertexTypeToString(VertexType type) {
        switch (type) {
            case VertexType::SERIAL: return "NORMAL";
            case VertexType::PARALLEL_MIXER: return "PARALLEL_MIXER";
            case VertexType::LAYER_MIXER: return "LAYER_MIXER";
            default: throw std::invalid_argument("Unknown vertex type");
        }
    }

    struct CoreVertexMeta {
        CoreVertexMeta() = default;
        explicit CoreVertexMeta(std::string_view name, VertexType type):
            name(name),
            type(type){}
        std::string name;
        VertexType type = VertexType::NONE;
        virtual std::string toString();
    };

    template<typename T>
    using InternalCoreGraph = boost::directed_graph<T, boost::no_property, boost::no_property>;

    template<typename T>
    class DirectedGraphWrapper: public InternalCoreGraph<T> {
        public:
            using Vertex = typename InternalCoreGraph<T>::vertex_descriptor;
            using VertexIterator = typename InternalCoreGraph<T>::vertex_iterator;
            using Edge = typename InternalCoreGraph<T>::edge_descriptor;
            using EdgeIterator = typename InternalCoreGraph<T>::edge_iterator;
        private:
            template<typename R>
            using VertexAccessor = std::function<R(const Vertex)>;

            template<typename R>
            using InternalVertexBundleIterator = boost::range_detail::transformed_range<VertexAccessor<R>, const std::pair<VertexIterator , VertexIterator>>;

            template<typename R>
            [[nodiscard]]
            VertexAccessor<R> generateAccessor() noexcept {
                return [map = boost::get(boost::vertex_bundle, *this)](const Vertex v) -> R {
                    return map[v];
                };
            }

            template<typename R>
            using EdgeAccessor = std::function<R(const Edge)>;

            template<typename R>
            [[nodiscard]]
            EdgeAccessor<R> generateOutEdgeAccessor() noexcept {
                return [this](const Edge e) -> R {
                    return boost::target(e, *this);
                };
            }

            template<typename R>
            [[nodiscard]]
            EdgeAccessor<R> generateInEdgeAccessor() noexcept {
                return [this](const Edge e) -> R {
                    return boost::source(e, *this);
                };
            }
        public:
            DirectedGraphWrapper() = default;

            using VertexBundleIterator = InternalVertexBundleIterator<T&>;

            [[nodiscard]]
            VertexBundleIterator vertexBundlesIterator() noexcept {
                return boost::vertices(*this) | boost::adaptors::transformed(generateAccessor<T&>());
            }

            [[nodiscard]]
            VertexBundleIterator neighbouringOutVertexBundlesIterator(const Vertex vertex) noexcept {
                return boost::out_edges(vertex, *this)
                       | boost::adaptors::transformed(generateOutEdgeAccessor<Vertex>())
                       | boost::adaptors::transformed(generateAccessor<T&>());
            }

            [[nodiscard]]
            VertexBundleIterator neighbouringInVertexBundlesIterator(const Vertex vertex) noexcept {
                return boost::in_edges(vertex, *this)
                       | boost::adaptors::transformed(generateInEdgeAccessor<Vertex>())
                       | boost::adaptors::transformed(generateAccessor<T&>());
            }

            VertexIterator findVertex(const std::string_view& name);
            [[nodiscard]]
            bool hasVertex(const std::string_view& name) const;
            void mergeGraphs(Vertex graph1Vertex,
                             const InternalCoreGraph<CoreVertexMeta>& graph2,
                             Vertex graph2Vertex);
            [[nodiscard]]
            bool hasInVertices(Vertex vertex);
            [[nodiscard]]
            bool hasOutVertices(Vertex vertex);
    };

    using CoreGraph = DirectedGraphWrapper<CoreVertexMeta>;
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
