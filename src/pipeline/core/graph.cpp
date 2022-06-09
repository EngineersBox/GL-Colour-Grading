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

//    template<typename T>
//    template<typename R>
//    typename DirectedGraphWrapper<T>::template Accessor<R> DirectedGraphWrapper<T>::generateAccessor() noexcept {
//        return [map = get(boost::vertex_bundle, *this)](const InternalVertex v) -> R {
//            return map[v];
//        };
//    }

//    template<typename T>
//    typename DirectedGraphWrapper<T>::VertexBundleIterator DirectedGraphWrapper<T>::vertexBundlesIterator() noexcept {
//        return boost::vertices(this) | boost::adaptors::transformed(generateAccessor<T&>());
//    }
//
//    template<typename T>
//    typename DirectedGraphWrapper<T>::VertexBundleConstIterator DirectedGraphWrapper<T>::vertexBundlesIterator() const noexcept {
//        return boost::vertices(this) | boost::adaptors::transformed(generateAccessor<const T&>());
//    }
}