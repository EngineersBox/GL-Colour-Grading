#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINEPASS_HPP
#define GL_COLOUR_GRADING_PIPELINEPASS_HPP

#include <memory>
#include <utility>

#include "buffers/vao.hpp"
#include "shaders/shader.hpp"
#include "texture.hpp"

namespace GLCG::Pipelines {
    // TODO/NOTE: Should this be refactored to just be an apply() method which can be overriden?
    //  seems like a lot of restriction around providing resources like textures, FBOs, EBOs, etc
    //  for other situations and definitions with the current model
    class PipelinePass {
        public:
            PipelinePass() = delete;
            PipelinePass(Device::GPU::Shaders::Shader shader,
                         Device::GPU::Buffers::VAO vao):
                shader(std::move(shader)),
                vao(std::move(vao)) {};
            virtual ~PipelinePass() = default;

            virtual void apply(const int width, const int height);
        protected:
            Device::GPU::Shaders::Shader shader;
            Device::GPU::Buffers::VAO vao;
    };
}

#endif //GL_COLOUR_GRADING_PIPELINEPASS_HPP
