#include "shader.hpp"

#include <spdlog/spdlog.h>
#include <iterator>
#include <algorithm>
#include "../../io/reader.hpp"

namespace GLCG::GPU::Shaders {
    void Shader::activate() const {
        glUseProgram(this->id);
    }

    void Shader::destroy() const {
        glDeleteProgram(this->id);
    }

    void logShaderError(unsigned int shader,
                        const ProgramType type,
                        const char* stage) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        spdlog::error(
            "[OpenGL] Shader {} error for \"{}\": {}",
            stage,
            programTypeToString(type),
            std::string(errorLog.begin(), errorLog.end())
        );
    }

    void Shader::compileErrors(unsigned int shader, const ProgramType type) {
        GLint hasCompiled;
        if (type != ProgramType::PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE) {
                logShaderError(
                    shader,
                    type,
                    "compilation"
                );
                exit(1);
            }
            return;
        }
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            logShaderError(
                shader,
                type,
                "linking"
            );
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

    ShaderBuilder Shader::builder() {
        return {};
    }

    ShaderBuilder::ShaderBuilder(): shader() {
        this->state = ShaderBuildState::CREATING_PROGRAM;
        this->shader.id = glCreateProgram();
    }

    ShaderBuilder& ShaderBuilder::markSeparable() {
        if (!isVersionSupported(4, 1)) {
            throw OpenGLFeatureSupportException(
                "ARB_separate_shader_objects",
                4, 1
            );
        } else if (this->state >= ShaderBuildState::LINKING_PROGRAM) {
            throw std::runtime_error("Cannot mark program as separable after linking");
        }
        glProgramParameteri(this->shader.id, GL_PROGRAM_SEPARABLE, GL_TRUE);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withVertex(const char* vertexFile) {
        attachShader(ProgramType::VERTEX, vertexFile);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withFragment(const char* fragmentFile) {
        attachShader(ProgramType::FRAGMENT, fragmentFile);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withGeometry(const char* geometryFile) {
        attachShader(ProgramType::GEOMETRY, geometryFile);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withCompute(const char* computeFile) {
        if (!isVersionSupported(4, 3)) {
            throw OpenGLFeatureSupportException(
                "ARB_compute_shader",
                4, 3
            );
        }
        attachShader(ProgramType::COMPUTE, computeFile);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withTessellationControl(const char* tessControlFile) {
        if (!isVersionSupported(4, 0)) {
            throw OpenGLFeatureSupportException(
                "ARB_tessellation_shader",
                4, 0
            );
        }
        attachShader(ProgramType::TESSELATION_CONTROL, tessControlFile);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withTessellationEvaluation(const char* tessEvalFile) {
        if (!isVersionSupported(4, 0)) {
            throw OpenGLFeatureSupportException(
                "ARB_tessellation_shader",
                4, 0
            );
        }
        attachShader(ProgramType::TESSELATION_EVALUATION, tessEvalFile);
        return *this;
    }

    void ShaderBuilder::attachShader(ProgramType type, const char *shaderFile) {
        this->state = ShaderBuildState::ATTACHING_SHADERS;
        if (shaderFile == nullptr) {
            throw std::invalid_argument("Shader file cannot be null");
        } else if (this->attachedShaders.contains(type)) {
            throw std::invalid_argument("Shader has already been attached");
        }
        std::string shaderCode = Reader::readFileIntoString(shaderFile);
        GLuint compiledShaderId = Shader::createCompiledShader(shaderCode.c_str(), type);
        glAttachShader(compiledShaderId, this->shader.id);
        this->attachedShaders[type] = compiledShaderId;
    }

    void ShaderBuilder::detachAttachedShaders() {
        this->state = ShaderBuildState::DETACHING_SHADERS;
        std::for_each(
            this->attachedShaders.begin(),
            this->attachedShaders.end(),
            [this](std::pair<ProgramType, GLuint> entry) -> void {
                glDetachShader(this->shader.id, entry.second);
            }
        );
    }

    void ShaderBuilder::deleteAttachedShaders() {
        this->state = ShaderBuildState::DELETING_SHADERS;
        std::for_each(
            this->attachedShaders.begin(),
            this->attachedShaders.end(),
            [](std::pair<ProgramType, GLuint> entry) -> void {
                glDeleteShader(entry.second);
            }
        );
    }

    ShaderBuilder::operator Shader() {
        this->state = ShaderBuildState::LINKING_PROGRAM;
        glLinkProgram(this->shader.id);
        Shader::compileErrors(this->shader.id, ProgramType::PROGRAM);
        detachAttachedShaders();
        deleteAttachedShaders();
        this->state = ShaderBuildState::DONE;
        return std::move(this->shader);
    }
}