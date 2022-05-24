#include "graph.hpp"

namespace GLCG::Pipelines {
    template<typename T>
    void GraphNode<T>::insertAfter(T&& newValue) {
        if (this->nextNodes.empty()) {
            this->nextNodes.push_back(std::make_shared(new GraphNode<T>(std::move(newValue), std::make_shared(this))));
            return;
        }
        /* TODO: Cases to handle:
         *  1. Single next node, insert serially (MAKE_SERIAL)
         *  2. Multiple next nodes, prefix all (MAKE_PARALLEL_PREFIXED)
         */
        if (this->nextNodes.size() == 1) {

        }
    }

    template<typename T>
    void GraphNode<T>::insertBefore(T&& newValue) {

    }

    template<typename T>
    constexpr bool GraphNode<T>::isSplitNode() const noexcept {
        return this->nextNodes.size() > 1;
    }

    template<typename T>
    constexpr bool GraphNode<T>::isJoinNode() const noexcept {
        return this->prevNodes.size() > 1;
    }

    template<typename T>
    constexpr bool GraphNode<T>::isNormalNode() const noexcept {
        return !isSplitNode() && !isJoinNode();
    }

    template<typename T>
    std::unique_ptr<T> GraphNode<T>::getvalue() {
        return std::make_unique(&this->value);
    }
}