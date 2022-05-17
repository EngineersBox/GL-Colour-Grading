#pragma once

#ifndef GL_COLOUR_GRADING_SHADER_HPP
#define GL_COLOUR_GRADING_SHADER_HPP

#include <glad/glad.h>

namespace GLCG::GPU::Shaders {
    typedef enum ProgramType {
        FRAGMENT,
        VERTEX,
        GEOMETRY,
        PROGRAM,
    } ProgramType;

    class Shader {
        public:
            GLuint id;
            Shader(const char* vertexFile, const char* fragmentFile);

            void activate() const;
            void remove() const;
        private:
            static void compileErrors(unsigned int shader, ProgramType type);
    };
}

#endif //GL_COLOUR_GRADING_SHADER_HPP
