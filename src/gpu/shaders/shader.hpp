#pragma once

#ifndef GL_COLOUR_GRADING_SHADER_HPP
#define GL_COLOUR_GRADING_SHADER_HPP

#include <glad/glad.h>
#include <exception>
#include <string>
#include <map>

#include "programType.hpp"
#include "exception/openGLFeatureSupportException.hpp"

namespace GLCG::GPU::Shaders {
    class ShaderBuilder;
    class Shader {
        public:
            GLuint id = 0;
            Shader() = default;

            void activate() const;
            void destroy() const;

            friend class ShaderBuilder;
            static ShaderBuilder builder();
            void validateProgram() const;
        private:
            static GLuint createCompiledShader(const char* source, ProgramType type);
            static void compileErrors(unsigned int shader, ProgramType type);
        protected:
            std::map<ProgramType, GLuint> attachedShaders;
    };
    enum class ShaderBuildState {
        NONE = -1,
        CREATING_PROGRAM = 0,
        ATTACHING_SHADERS = 1,
        LINKING_PROGRAM = 2,
        DETACHING_SHADERS = 3,
        DELETING_SHADERS = 4,
        DONE = 5
    };
    class ShaderBuilder {
        public:
            ShaderBuilder();

            ShaderBuilder& markSeparable();
            ShaderBuilder& withVertex(const char* vertexFile);
            ShaderBuilder& withFragment(const char* fragmentFile);
            ShaderBuilder& withGeometry(const char* geometryFile);
            ShaderBuilder& withCompute(const char* computeFile);
            ShaderBuilder& withTessellationControl(const char* tessControlFile);
            ShaderBuilder& withTessellationEvaluation(const char* tessEvalFile);

            operator Shader();

            [[nodiscard]]
            constexpr ShaderBuildState getState() const noexcept {
                return this->state;
            }
        private:
            void attachShader(ProgramType type, const char* shaderFile);
            void detachAttachedShaders();
            void deleteAttachedShaders();
        protected:
            ShaderBuildState state = ShaderBuildState::NONE;
            Shader shader;
    };
}

#endif //GL_COLOUR_GRADING_SHADER_HPP
