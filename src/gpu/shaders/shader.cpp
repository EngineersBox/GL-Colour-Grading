#include "shader.hpp"

#include <spdlog/spdlog.h>
#include "../../io/reader.hpp"

namespace GLCG::GPU::Shaders {
    Shader::Shader(const char* vertexFile, const char* fragmentFile) {
        std::string vertexCode = Reader::readFileIntoString(vertexFile);
        std::string fragmentCode = Reader::readFileIntoString(fragmentFile);

        GLuint vertexShader = createCompiledShader(vertexCode.c_str(), ProgramType::VERTEX);
        GLuint fragmentShader = createCompiledShader(fragmentCode.c_str(), ProgramType::FRAGMENT);

        this->id = glCreateProgram();
        glAttachShader(this->id, vertexShader);
        glAttachShader(this->id, fragmentShader);
        glLinkProgram(this->id);
        compileErrors(this->id, ProgramType::PROGRAM);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::activate() const {
        glUseProgram(this->id);
    }

    void Shader::destroy() const {
        glDeleteProgram(this->id);
    }

    void Shader::compileErrors(unsigned int shader, const ProgramType type) {
        GLint hasCompiled;
        char infoLog[1024];
        if (type != ProgramType::PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                spdlog::error("[OpenGL] Shader compilation error for \"{}\": {}", programTypeToString(type), infoLog);
                exit(1);
            }
            return;
        }
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            spdlog::error("[OpenGL] Shader linking error for \"{}\": {}", programTypeToString(type), infoLog);
            exit(1);
        }
    }

    GLuint Shader::createCompiledShader(const char* source, const ProgramType type) {
        GLuint shader = glCreateShader(programTypeToGLenum(type));
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        compileErrors(shader, type);
        return shader;
    }
}