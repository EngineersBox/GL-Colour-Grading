#include "graph.hpp"

#include <stdexcept>

namespace GLCG::Pipelines {
    template<typename T>
    void GraphNode<T>::insertAfter(std::string newName, T&& newValue) {
        if (this->nextNodes.empty()) {
            this->nextNodes[newName] = std::make_shared(new GraphNode<T>(
                newName,
                std::move(newValue),
                std::make_shared(this)
            ));
            return;
        } else if (isNormalNode()) {
            if (this->nextNodes.contains(newName)) {
                throw std::runtime_error("Node already exists");
            }
            std::shared_ptr<GraphNode<T>> node = std::make_shared(new GraphNode<T>(
                newName,
                std::move(newValue),
                std::make_pair(this->name, std::make_shared(this)),
                std::make_pair(
                    this->nextNodes.begin()->second->name,
                    std::make_shared(this->nextNodes.begin()->second)
                )
            ));
            this->nextNodes.begin()->second->prevNodes.erase(this->name);
            this->nextNodes.begin()->second->prevNodes[newName] = std::make_shared(node);
            this->nextNodes[newName] = std::make_shared(node);
            return;
        }
        for (auto [nodeName, nodePtr] : this->nextNodes) {

        }
    }

    template<typename T>
    void GraphNode<T>::insertBefore(std::string newName, T&& newValue) {

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
    std::unique_ptr<T> GraphNode<T>::getValue() {
        return std::make_unique(&this->value);
    }
}