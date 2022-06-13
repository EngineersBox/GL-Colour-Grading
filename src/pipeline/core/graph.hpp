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

            using EdgeTargetAccessor = std::function<InternalVertex(const InternalEdge)>;

            [[nodiscard]]
            EdgeTargetAccessor generateEdgeTargetAccessor() noexcept {
                return [this](const InternalEdge edge) -> InternalVertex {
                    return boost::target(edge, *this);
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
                return boost::out_edges(vertex, *this)
                    | boost::adaptors::transformed(generateEdgeTargetAccessor())
                    | boost::adaptors::transformed(generateAccessor<T&>());
            }
    };

    using CoreGraph = DirectedGraphWrapper<CoreVertexMeta>;

    namespace Graph {
        static CoreGraph::VertexIterator findVertex(const CoreGraph& graph, const std::string_view& name) {
            boost::iterator_range<CoreGraph::VertexIterator> iter = boost::make_iterator_range(vertices(graph));
            return std::find_if(
                iter.begin(),
                iter.end(),
                [&graph, &name](const CoreGraph::Vertex& current) -> bool{
                    return graph[current].name == name;
                }
            );
        }

        static bool hasVertex(const CoreGraph& graph, const std::string_view& name) {
            boost::iterator_range<CoreGraph::VertexIterator> iter = boost::make_iterator_range(vertices(graph));
            return std::any_of(
                iter.begin(),
                iter.end(),
                [&graph, name](const CoreGraph::Vertex& current) -> bool {
                    return graph[current].name == name;
                }
            );
        }

        static void mergeGraphs(InternalCoreGraph<CoreVertexMeta>& graph1,
                                CoreGraph::Vertex graph1Vertex,
                                const InternalCoreGraph<CoreVertexMeta>& graph2,
                                CoreGraph::Vertex graph2Vertex) {
            std::vector<CoreGraph::Vertex> orig2copy_data(num_vertices(graph2));
            auto mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, graph2));
            boost::copy_graph(graph2, graph1, boost::orig_to_copy(mapV));
            CoreGraph::Vertex graph2SourceVertex = mapV[graph2Vertex];
            boost::add_edge(graph1Vertex, graph2SourceVertex, graph1);
        }
    }
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
