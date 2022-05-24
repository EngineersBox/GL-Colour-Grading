#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <type_traits>
#include <list>
#include <vector>
#include <memory>

namespace GLCG::Pipelines {
    enum class GraphNodeModifyBehaviour {
        MAKE_SERIAL = 0,
        MAKE_PARALLEL_PREFIXED = 1,
    };
    template<typename T>
    struct GraphNode {
        public:
            explicit GraphNode(T&& value, std::shared_ptr<GraphNode<T>> prev = nullptr, std::shared_ptr<GraphNode<T>> next = nullptr): value(std::move(value)) {
                if (prev != nullptr) {
                    this->prevNodes.push_back(std::move(prev));
                }
                if (next != nullptr) {
                    this->nextNodes.push_back(std::move(next));
                }
            }

            void insertAfter(T&& newValue);
            void insertBefore(T&& newValue);

            [[nodiscard]]
            constexpr bool isSplitNode() const noexcept;

            [[nodiscard]]
            constexpr bool isJoinNode() const noexcept;

            [[nodiscard]]
            constexpr bool isNormalNode() const noexcept;

            std::unique_ptr<T> getvalue();

            std::vector<std::shared_ptr<GraphNode<T>>> nextNodes;
            std::vector<std::shared_ptr<GraphNode<T>>> prevNodes;
        private:
            T value;
    };

    template<typename T>
    class Graph {
        public:
            // constructor
            Graph() = default;
        private:
            std::shared_ptr<GraphNode<T>> head;
            std::shared_ptr<GraphNode<T>> tail;
    };
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
