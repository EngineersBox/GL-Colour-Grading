#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINEPASS_HPP
#define GL_COLOUR_GRADING_PIPELINEPASS_HPP

#include <memory>
#include <utility>

#include "buffers/vao.hpp"
#include "shaders/shader.hpp"
#include "texture.hpp"

namespace GLCG::Pipelines {
    class PipelinePass {
        public:
            PipelinePass() = delete;
            PipelinePass(GPU::Shaders::Shader shader,
                         GPU::Buffers::VAO vao):
                shader(std::move(shader)),
                vao(std::move(vao)) {};
            virtual ~PipelinePass() = default;

            virtual void apply();
        private:
            GPU::Shaders::Shader shader;
            GPU::Buffers::VAO vao;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINEPASS_HPP
