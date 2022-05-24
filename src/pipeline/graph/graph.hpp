#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <type_traits>
#include <list>
#include <vector>

namespace GLCG::Pipelines {
    enum class GraphNodeType {
        NORMAL,
        PARALLEL,
        JOIN
    };
    template<typename T>
    struct GraphNode {
        explicit GraphNode(T value):
            value(value),
            type(GraphNodeType::NORMAL) {}

        explicit GraphNode(T value, GraphNodeType type):
            value(std::move(value)),
            type(type){}
        T value;
        GraphNodeType type;
    };

    template<typename T>
    using Graph = std::list<GraphNode<T>>;

    template<typename T>
    struct ParallelGraphNode: public GraphNode<std::vector<Graph<T>>> {
        explicit ParallelGraphNode(std::vector<Graph<T>> value):
            GraphNode<std::vector<Graph<T>>>(value, GraphNodeType::PARALLEL) {}
    };

    template<typename T, typename O> requires std::is_enum_v<O>
    struct JoinGraphNode: public GraphNode<T> {
        explicit JoinGraphNode(T value, O joinOperation):
            GraphNode<T>(value, GraphNodeType::JOIN),
            joinOperation(joinOperation) {}
        O joinOperation;
    };
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
