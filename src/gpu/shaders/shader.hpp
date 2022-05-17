#pragma once

#ifndef GL_COLOUR_GRADING_SHADER_HPP
#define GL_COLOUR_GRADING_SHADER_HPP

#include <glad/glad.h>

namespace GLCG::GPU::Shaders {
    class Shader {
        public:
            GLuint id;
            Shader(const char* vertexFile, const char* fragmentFile);

            void Activate();
            void Delete();
    };
}

#endif //GL_COLOUR_GRADING_SHADER_HPP
