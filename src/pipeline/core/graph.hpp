#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <boost/range/adaptors.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>
#include <utility>

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
        explicit CoreVertexMeta(std::string const& name, VertexType type):
            name(name),
            type(type) {}
        std::string name;
        VertexType type = VertexType::NONE;
        virtual std::string toString();
    };

    template<typename T>
    using InternalCoreGraph = boost::directed_graph<T, boost::vecS, boost::vecS>;

    template<typename T>
    class DirectedGraphWrapper: public InternalCoreGraph<T> {
        private:
            using InternalVertex = typename InternalCoreGraph<T>::vertex_descriptor;
            using InternalVertexIterator = typename InternalCoreGraph<T>::vertex_iterator;

            struct Accessor: std::unary_function<const InternalVertex, T&> {
                explicit Accessor(const boost::property_map<InternalCoreGraph<T>, T> bundleMap):
                    bundleMap(bundleMap),
                    std::unary_function<const InternalVertex, T&>() {}
                T& operator()(const InternalVertex v) noexcept {
                    return this->bundleMap[v];
                }
                T& operator()(const InternalVertex v) const noexcept {
                    return this->bundleMap[v];
                }
                private:
                    const boost::property_map<InternalCoreGraph<T>, T> bundleMap;
            };
        public:
            using VertexBundleIterator = boost::transformed_range<Accessor, std::pair<InternalVertexIterator, InternalVertexIterator>>;
            using VertexBundleConstIterator = boost::transformed_range<Accessor, std::pair<InternalVertexIterator, InternalVertexIterator>>;

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
            boost::iterator_range<CoreGraph::vertex_iterator> iter = boost::make_iterator_range(vertices(graph));
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
