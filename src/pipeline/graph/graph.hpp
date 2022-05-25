#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <type_traits>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace GLCG::Pipelines {
    template<typename T>
    struct GraphNode {
        public:
            explicit GraphNode(std::string&& name,
                               T&& value,
                               std::pair<std::string, std::shared_ptr<GraphNode<T>>> prev = nullptr,
                               std::pair<std::string, std::shared_ptr<GraphNode<T>>> next = nullptr):
                    name(std::move(name)),
                    value(std::move(value)) {
                if (prev != nullptr) {
                    this->prevNodes[prev.first] = std::make_shared(prev.second);
                }
                if (next != nullptr) {
                    this->nextNodes[next.first] = std::make_shared(next.second);
                }
            }

            void insertAfter(std::string newName, T&& newValue);
            void insertBefore(std::string newName, T&& newValue);

            [[nodiscard]]
            constexpr bool isSplitNode() const noexcept;

            [[nodiscard]]
            constexpr bool isJoinNode() const noexcept;

            [[nodiscard]]
            constexpr bool isNormalNode() const noexcept;

            std::unique_ptr<T> getValue();

            std::map<std::string, std::shared_ptr<GraphNode<T>>> nextNodes;
            std::map<std::string, std::shared_ptr<GraphNode<T>>> prevNodes;
            std::string name;
        private:
            T value;
    };

    template<typename T>
    class Graph {
        public:
            // constructor
            Graph() = default;
        private:
            std::pair<std::string, std::shared_ptr<GraphNode<T>>> head;
            std::pair<std::string, std::shared_ptr<GraphNode<T>>> tail;
    };
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
