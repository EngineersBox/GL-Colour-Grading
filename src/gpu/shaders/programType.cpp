#include "programType.hpp"

#include <stdexcept>

namespace GLCG::GPU::Shaders {
    std::string programTypeToString(ProgramType type) {
        switch (type) {
            case ProgramType::FRAGMENT: return "FRAGMENT";
            case ProgramType::VERTEX: return "VERTEX";
            case ProgramType::GEOMETRY: return "GEOMETRY";
            case ProgramType::COMPUTE: return "COMPUTE";
            case ProgramType::TESSELATION_CONTROL: return "TESSELATION_CONTROL";
            case ProgramType::TESSELATION_EVALUATION: return "TESSELATION_EVALUATION";
            default: return "PROGRAM";
        }
    }

    GLenum programTypeToGLenum(ProgramType type) {
        switch (type) {
            case ProgramType::FRAGMENT: return GL_FRAGMENT_SHADER;
            case ProgramType::VERTEX: return GL_VERTEX_SHADER;
            case ProgramType::GEOMETRY: return GL_GEOMETRY_SHADER;
            case ProgramType::COMPUTE: return GL_COMPUTE_SHADER;
            case ProgramType::TESSELATION_CONTROL: return GL_TESS_CONTROL_SHADER;
            case ProgramType::TESSELATION_EVALUATION: return GL_TESS_EVALUATION_SHADER;
            default: throw std::invalid_argument("No such GLenum equivalent for program type");
        }
    }
}