#pragma once

#ifndef GL_COLOUR_GRADING_PROGRAMTYPE_HPP
#define GL_COLOUR_GRADING_PROGRAMTYPE_HPP

#include <glad/glad.h>
#include <string>

namespace GLCG::GPU::Shaders {
    using ProgramType = enum class ProgramType {
        FRAGMENT,
        VERTEX,
        GEOMETRY,
        COMPUTE,
        TESSELATION_CONTROL,
        TESSELATION_EVALUATION,
        PROGRAM,
    };
    std::string programTypeToString(ProgramType type);
    GLenum programTypeToGLenum(ProgramType type);
}

#endif //GL_COLOUR_GRADING_PROGRAMTYPE_HPP
