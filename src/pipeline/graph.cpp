#include "graph.hpp"

#include "../util/stringUtils.hpp"

namespace GLCG::Pipelines {
    std::string CoreVertexMeta::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str()
        );
    }
}