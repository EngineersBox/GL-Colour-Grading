#include "shader.hpp"

#include <spdlog/spdlog.h>
#include "../../io/reader.hpp"

namespace GLCG::GPU::Shaders {
    Shader::Shader(const char* vertexFile, const char* fragmentFile) {
        std::string vertexCode = Reader::readFileIntoString(vertexFile);
        std::string fragmentCode = Reader::readFileIntoString(fragmentFile);

        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);
        compileErrors(vertexShader, ProgramType::VERTEX);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        compileErrors(fragmentShader, ProgramType::FRAGMENT);

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

    void Shader::remove() const {
        glDeleteProgram(this->id);
    }

    void Shader::compileErrors(unsigned int shader, const ProgramType type) {
        GLint hasCompiled;
        char infoLog[1024];
        if (type != ProgramType::PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                spdlog::error("[OpenGL] Shader compilation error for \"{}\": {}", type, infoLog);
                exit(1);
            }
            return;
        }
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            spdlog::error("[OpenGL] Shader linking error for \"{}\": {}", type, infoLog);
            exit(1);
        }
    }
}