#include "coreGraph.hpp"

namespace GLCG::Pipelines {
    template<typename T>
    void ParallelNode<T>::addParallelGraph(CoreGraph<T>& graph) noexcept {
        this->values.push_back({ .graph = graph });
    }
}