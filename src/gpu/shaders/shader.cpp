#include "shader.hpp"

#include "../../io/reader.hpp"

namespace GLCG::GPU::Shaders {
    Shader::Shader(const char* vertexFile, const char* fragmentFile) {
        std::string vertexCode = Reader::readFileIntoString(vertexFile);
        std::string fragmentCode = Reader::readFileIntoString(fragmentFile);

        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        this->id = glCreateProgram();
        glAttachShader(this->id, vertexShader);
        glAttachShader(this->id, fragmentShader);
        glLinkProgram(this->id);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

    void Shader::Activate() {
        glUseProgram(this->id);
    }

    void Shader::Delete() {
        glDeleteProgram(this->id);
    }
}