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
    bool DirectedGraphWrapper<T>::hasInVertices(Vertex vertex) const {
        return boost::in_degree(vertex, *this) > 0;
    }

    template<typename T>
    bool DirectedGraphWrapper<T>::hasOutVertices(Vertex vertex) const {
        return boost::out_degree(vertex, *this) > 0;
    }
}