#pragma once

#ifndef GL_COLOUR_GRADING_GRADER_HPP
#define GL_COLOUR_GRADING_GRADER_HPP

#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <libconfig.h++>

#include "config.hpp"

namespace GLCG::Core {
    class Grader {
        public:
            explicit Grader(const char* filePath);

            void init();
            void destroy();

            [[nodiscard]] constexpr GLFWwindow* getWindow() const {
                return this->window;
            }
            [[nodiscard]] constexpr int getWidth() const {
                return this->config.window.width;
            }
            [[nodiscard]] constexpr int getHeight() const {
                return this->config.window.height;
            }
        private:
            GLFWwindow* window = nullptr;
            Config config;

            void initGLContext();
            void destroyGLContext();
    };
}

#endif //GL_COLOUR_GRADING_GRADER_HPP
