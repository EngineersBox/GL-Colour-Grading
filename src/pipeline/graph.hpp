#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>

namespace GLCG::Pipelines {
    enum class VertexType {
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
        VertexType type;
        virtual std::string toString();
    };

    using CoreGraph = boost::directed_graph<CoreVertexMeta, boost::no_property, boost::no_property>;
    using Vertex = CoreGraph::vertex_descriptor;
    using Edge = CoreGraph::edge_descriptor;

    static void mergeGraphs(CoreGraph& graph1, Vertex graph1Vertex, const CoreGraph& graph2, Vertex graph2Vertex) {
        std::vector<Vertex> orig2copy_data(num_vertices(graph2));
        auto mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, graph2));
        boost::copy_graph(graph2, graph1, boost::orig_to_copy(mapV));
        Vertex graph2SourceVertex = mapV[graph2Vertex];
        boost::add_edge(graph1Vertex, graph2SourceVertex, graph1);
    }
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
