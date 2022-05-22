#pragma once

#ifndef GL_COLOUR_GRADING_OPENGLUTILS_HPP
#define GL_COLOUR_GRADING_OPENGLUTILS_HPP

#include <glad/glad.h>
#include <string>
#include <spdlog/spdlog.h>

namespace GLCG::Utils::OpenGL {
    static void _glCheckError(const char *file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            std::string error;
            switch (errorCode) {
                case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
                case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
                default: error = "UNKNOWN ERROR"; break;
            }
            spdlog::error("[OpenGL] Error occurred at {}({}): {}", file, line - 1, error);
            exit(1);
        }
    }
    [[nodiscard]]
    static bool isVersionSupported(const int major, const int minor) {
        int glMajorVersion;
        int glMinorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);
        return glMajorVersion > major || (glMajorVersion == major && glMinorVersion >= minor);
    }
}

#define glCheckError() GLCG::Utils::OpenGL::_glCheckError(__FILE__, __LINE__)

#endif //GL_COLOUR_GRADING_OPENGLUTILS_HPP
