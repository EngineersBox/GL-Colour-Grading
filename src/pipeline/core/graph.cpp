#include "graph.hpp"

#include "stringUtils.hpp"

namespace GLCG::Pipelines {
    std::string CoreVertexMeta::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str()
        );
    }

    template<typename T>
    typename DirectedGraphWrapper<T>::VertexBundleIterator DirectedGraphWrapper<T>::vertexBundlesIterator() noexcept {
        return vertices(this) | boost::adaptors::transformed(Accessor(get(boost::vertex_bundle, this)));
    }

    template<typename T>
    typename DirectedGraphWrapper<T>::VertexBundleConstIterator DirectedGraphWrapper<T>::vertexBundlesIterator() const noexcept {
        return vertices(this) | boost::adaptors::transformed(Accessor(get(boost::vertex_bundle, this)));
    }
}