#include "pipelinePass.hpp"

namespace GLCG::Pipelines {
    void PipelinePass::apply(const int width, const int height) {
        this->shader.activate();
        this->vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}