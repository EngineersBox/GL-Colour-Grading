#include "pipelinePass.hpp"

namespace GLCG::Pipelines {
    void PipelinePass::apply() {
        this->shader.activate();
        this->vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}