#pragma once

#ifndef GL_COLOUR_GRADING_SHADER_HPP
#define GL_COLOUR_GRADING_SHADER_HPP

#include <glad/glad.h>
#include <exception>
#include <string>
#include "programType.hpp"

namespace GLCG::GPU::Shaders {
    class Shader {
        public:
            GLuint id;
            Shader(const char* vertexFile, const char* fragmentFile);

            void activate() const;
            void destroy() const;

        private:
            static GLuint createCompiledShader(const char* source, ProgramType type);
            static void compileErrors(unsigned int shader, ProgramType type);
    };
}

#endif //GL_COLOUR_GRADING_SHADER_HPP
