#include "grader.hpp"

#include <spdlog/spdlog.h>

#include "../logging/logger.hpp"

namespace GLCG::Core {
    Grader::Grader(const char* filePath): config(filePath) {}

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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->config.openGL.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->config.openGL.minorVersion);
        // Using core profile for only modern APIs
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(
            this->getWidth(),
            this->getHeight(),
            this->config.window.title.c_str(),
            nullptr,
            nullptr
        );
        if (this->window == nullptr) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
            exit(1);
        }
        // Contextualise the current window to draw to
        glfwMakeContextCurrent(this->window);
        glfwSetFramebufferSizeCallback(
            this->window,
            GLCG::Core::Grader::framebufferSizeCallback
        );

        gladLoadGL();
        spdlog::trace("OpenGL Version: {}", glGetString(GL_VERSION));
        glViewport(0, 0, this->getWidth(), this->getHeight());
    }

    void Grader::destroyGLContext() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
        spdlog::info("Cleaned up GLFW/GLAD/OpenGL resources");
    }

    void Grader::framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }
}