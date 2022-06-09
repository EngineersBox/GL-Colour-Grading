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

            template<typename R>
            using Accessor = std::function<R(const InternalVertex)>;

            template<typename R>
            using InternalVertexBundleIterator = boost::range_detail::transformed_range<Accessor<R>, const std::pair<InternalVertexIterator, InternalVertexIterator>>;

            template<typename R>
            [[nodiscard]]
            Accessor<R> generateAccessor() noexcept;
        public:
            using VertexBundleIterator = InternalVertexBundleIterator<T&>;
            using VertexBundleConstIterator = InternalVertexBundleIterator<const T&>;

            [[nodiscard]]
            VertexBundleIterator vertexBundlesIterator() noexcept;

            [[nodiscard]]
            VertexBundleConstIterator vertexBundlesIterator() const noexcept;
    };

    using CoreGraph = DirectedGraphWrapper<CoreVertexMeta>;
    using Vertex = CoreGraph::vertex_descriptor;
    using VertexIterator = CoreGraph::vertex_iterator;
    using Edge = CoreGraph::edge_descriptor;
    using EdgeIterator = CoreGraph::edge_iterator;

    namespace Graph {
        static VertexIterator findVertex(const CoreGraph& graph, const std::string_view& name) {
            boost::iterator_range<VertexIterator> iter = boost::make_iterator_range(vertices(graph));
            return std::find_if(
                iter.begin(),
                iter.end(),
                [&graph, &name](const Vertex& current) -> bool{
                    return graph[current].name == name;
                }
            );
        }

        static bool hasVertex(const CoreGraph& graph, const std::string_view& name) {
            boost::iterator_range<VertexIterator> iter = boost::make_iterator_range(vertices(graph));
            return std::any_of(
                iter.begin(),
                iter.end(),
                [&graph, name](const Vertex& current) -> bool {
                    return graph[current].name == name;
                }
            );
        }

        static void mergeGraphs(InternalCoreGraph<CoreVertexMeta>& graph1, Vertex graph1Vertex, const InternalCoreGraph<CoreVertexMeta>& graph2, Vertex graph2Vertex) {
            std::vector<Vertex> orig2copy_data(num_vertices(graph2));
            auto mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, graph2));
            boost::copy_graph(graph2, graph1, boost::orig_to_copy(mapV));
            Vertex graph2SourceVertex = mapV[graph2Vertex];
            boost::add_edge(graph1Vertex, graph2SourceVertex, graph1);
        }
    }
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
