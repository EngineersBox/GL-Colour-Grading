#pragma once

#ifndef GL_COLOUR_GRADING_GRADER_HPP
#define GL_COLOUR_GRADING_GRADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLCG {
    static constexpr int WINDOW_WIDTH = 1920;
    static constexpr int WINDOW_HEIGHT = 1080;
    static constexpr int OPENGL_MAJOR_VERSION = 3;
    static constexpr int OPENGL_MINOR_VERSION = 3;
    static constexpr char WINDOW_TITLE[] = "GL Rendering";

    class Grader {
        public:
            Grader() {}

            void init();
            void destroy();

        [[nodiscard]] GLFWwindow* getWindow() const {
            return this->window;
        }
        [[nodiscard]] constexpr int getWidth() const {
            return WINDOW_WIDTH;
        }
        [[nodiscard]] constexpr int getHeight() const {
            return WINDOW_HEIGHT;
        }
        private:
            GLFWwindow* window = nullptr;

            void initGLContext();
            void destroyGLContext();
    };
}

#endif //GL_COLOUR_GRADING_GRADER_HPP
