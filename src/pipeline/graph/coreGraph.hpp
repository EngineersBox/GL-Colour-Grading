#pragma once

#ifndef GL_COLOUR_GRADING_COREGRAPH_HPP
#define GL_COLOUR_GRADING_COREGRAPH_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <list>
#include <concepts>
#include <memory>

namespace GLCG::Pipelines {
    enum class NodeType {
        CORE,
        NORMAL,
        PARALLEL,
        JOIN,
    };

    template<typename T>
    class CoreNode;

    template<typename T>
    using CoreGraph = std::list<CoreNode<T>>;

    template<typename T>
    class CoreNode {
        public:
            explicit CoreNode(NodeType type) {
                this->type = type;
            }

            NodeType type = NodeType::CORE;
    };

    template<typename T>
    class NormalNode: protected CoreNode<T> {
        public:
            explicit NormalNode(T value):
                value(value),
                CoreNode<T>(NodeType::NORMAL) {}

            T value;
    };

    template<typename T>
    class ParallelNode: protected CoreNode<T> {
        public:
            explicit ParallelNode(std::vector<CoreGraph<T>> graphs):
                values(graphs),
                CoreNode<T>(NodeType::PARALLEL) {}

            void addParallelGraph(CoreGraph<T>& graph) noexcept;

            using NodeIterator = typename std::vector<CoreGraph<T>>::iterator;
            using NodeConstIterator = typename std::vector<CoreGraph<T>>::const_iterator;
            using NodeReverseIterator = typename std::vector<CoreGraph<T>>::reverse_iterator;
            using NodeConstReverseIterator = typename std::vector<CoreGraph<T>>::const_reverse_iterator;

            [[nodiscard]]
            constexpr NodeIterator begin() noexcept {
                return this->values.begin();
            }

            [[nodiscard]]
            constexpr NodeIterator end() noexcept {
                return this->values.end();
            }

            [[nodiscard]]
            constexpr NodeConstIterator begin() const noexcept {
                return this->values.begin();
            }

            [[nodiscard]]
            constexpr NodeConstIterator end() const noexcept {
                return this->values.end();
            }

            [[nodiscard]]
            constexpr NodeReverseIterator rbegin() noexcept {
                return this->values.rbegin();
            }

            [[nodiscard]]
            constexpr NodeReverseIterator rend() noexcept {
                return this->values.rend();
            }

            [[nodiscard]]
            constexpr NodeConstReverseIterator rbegin() const noexcept {
                return this->values.rbegin();
            }

            [[nodiscard]]
            constexpr NodeConstReverseIterator rend() const noexcept {
                return this->values.rend();
            }
        private:
            std::vector<CoreGraph<T>> values;
    };

    template<typename T, typename O> requires std::is_enum_v<O>
    class JoinNode: protected CoreNode<T> {
        public:
            JoinNode(T value, O enumType):
                value(value),
                enumType(enumType),
                CoreNode<T>(NodeType::JOIN) {}

            O enumType;
            T value;
    };
}

#endif //GL_COLOUR_GRADING_COREGRAPH_HPP
