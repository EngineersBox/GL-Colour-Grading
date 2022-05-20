#include "grader.hpp"

#include <spdlog/spdlog.h>

#include "../logging/logger.hpp"

namespace GLCG {
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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
        // Using core profile for only modern APIs
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            WINDOW_TITLE,
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
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void Grader::destroyGLContext() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
        spdlog::debug("Cleaned up GLFW/GLAD/OpenGL resources");
    }
}