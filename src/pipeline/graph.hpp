#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
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

    using CoreGraph = boost::directed_graph<CoreVertexMeta, boost::no_property, boost::no_property>;
    using Vertex = CoreGraph::vertex_descriptor;
    using VertexIterator = CoreGraph::vertex_iterator;
    using Edge = CoreGraph::edge_descriptor;
    using EdgeIterator = CoreGraph::edge_iterator;

    namespace Graph {
        static VertexIterator findVertex(const CoreGraph& graph, const std::string_view& name) {
            VertexIterator iter, iterEnd;
            for (boost::tie(iter, iterEnd) = vertices(graph); iter != iterEnd; ++iter) {
                if (graph[*iter].name == name) return iter;
            }
            return iterEnd;
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

        static void mergeGraphs(CoreGraph& graph1, Vertex graph1Vertex, const CoreGraph& graph2, Vertex graph2Vertex) {
            std::vector<Vertex> orig2copy_data(num_vertices(graph2));
            auto mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, graph2));
            boost::copy_graph(graph2, graph1, boost::orig_to_copy(mapV));
            Vertex graph2SourceVertex = mapV[graph2Vertex];
            boost::add_edge(graph1Vertex, graph2SourceVertex, graph1);
        }
    }
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
