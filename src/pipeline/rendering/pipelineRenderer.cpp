#include "pipelineRenderer.hpp"

#include <stdexcept>

namespace GLCG:: Pipelines {
    void PipelineRenderer::render() {
        for (const CoreVertexMeta& vertex : this->pipeline->getVertexBundleIterator()) {
            switch (vertex.type) {
                case VertexType::NORMAL: renderNormalPass(dynamic_cast<const NormalVertex&>(vertex)); break;
                case VertexType::BLEND: renderBlendPass(dynamic_cast<const BlendVertex&>(vertex)); break;
                default: std::runtime_error(Utils::String::format(
                    "Cannot render pipeline pass \"%s\" with no specified type",
                    vertex.name.c_str()
                ));
            }
        }
    }

    void PipelineRenderer::renderBlendPass(const BlendVertex& blendVertex) {

    }

    void PipelineRenderer::renderNormalPass(const NormalVertex& normalVertex) {

    }
}