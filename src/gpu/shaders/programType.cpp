#include "programType.hpp"

namespace GLCG::GPU::Shaders {
    std::string programTypeToString(ProgramType type) {
        switch (type) {
            case ProgramType::FRAGMENT: return "FRAGMENT";
            case ProgramType::VERTEX: return "VERTEX";
            case ProgramType::GEOMETRY: return "GEOMETRY";
            default: return "PROGRAM";
        }
    }

    GLenum programTypeToGLenum(ProgramType type) {
        switch (type) {
            case ProgramType::FRAGMENT: return GL_FRAGMENT_SHADER;
            case ProgramType::VERTEX: return GL_VERTEX_SHADER;
            case ProgramType::GEOMETRY: return GL_GEOMETRY_SHADER;
            default: throw std::exception("No such GLenum equivalent for program type");
        }
    }
}