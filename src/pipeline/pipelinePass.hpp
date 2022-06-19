#pragma once

#ifndef GL_COLOUR_GRADING_PIPELINEPASS_HPP
#define GL_COLOUR_GRADING_PIPELINEPASS_HPP

#include <memory>
#include <utility>
#include <functional>

#include "buffers/vao.hpp"
#include "shaders/shader.hpp"
#include "texture.hpp"

namespace GLCG::Pipelines {
    // TODO/NOTE: Should this be refactored to just be an apply() method which can be overriden?
    //  seems like a lot of restriction around providing resources like textures, FBOs, EBOs, etc
    //  for other situations and definitions with the current model
    using PipelinePass = std::function<void(const int, const int)>;
}

#endif //GL_COLOUR_GRADING_PIPELINEPASS_HPP
