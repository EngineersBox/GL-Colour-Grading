#include "grader.hpp"

#include <spdlog/spdlog.h>

#include "../logging/logger.hpp"

namespace GLCG {
    Grader::Grader(const char* filePath) {
        this->config.readFile(filePath);
        if (!this->config.lookupValue("window.width", this->width)) {
            throw std::exception("Unable to read 'window.width' config property");
        } else if (!this->config.lookupValue("window.height", this->height)) {
            throw std::exception("Unable to read 'window.height' config property");
        } else if (!this->config.lookupValue("window.title", this->windowTitle)) {
            throw std::exception("Unable to read 'window.title' config property");
        } else if (!this->config.lookupValue("opengl.major_version", this->openGlMajorVersion)) {
            throw std::exception("Unable to read 'opengl.major_version' config property");
        } else if (!this->config.lookupValue("opengl.minor_version", this->openGlMinorVersion)) {
            throw std::exception("Unable to read 'opengl.minor_version' config property");
        }
        spdlog::debug("Loaded config from: {}", filePath);
    }

    void Grader::init() {
        this->initGLContext();
    }

    void Grader::destroy() {
        this->destroyGLContext();
    }

    void Grader::initGLContext() {
        glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(Logger::errorCallbackGLFW));
        glfwInit();
        spdlog::debug("Initialised GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->openGlMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->openGlMinorVersion);
        // Using core profile for only modern APIs
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(
            this->width,
            this->height,
            this->windowTitle.c_str(),
            nullptr,
            nullptr
        );
        if (this->window == nullptr) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
            exit(1);
        }
        // Contextualise the current window to draw to
        glfwMakeContextCurrent(window);

        gladLoadGL();
        spdlog::trace("OpenGL Version: {}", glGetString(GL_VERSION));
        glViewport(0, 0, this->width, this->height);
    }

    void Grader::destroyGLContext() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
        spdlog::info("Cleaned up GLFW/GLAD/OpenGL resources");
    }
}