#include "graph.hpp"

namespace GLCG::Pipelines {
    std::string CoreVertexMeta::toString() {
        std::stringstream ss;
        ss << "[TYPE: " << vertexTypeToString(this->type) << "]";
        return ss.str();
    }
}