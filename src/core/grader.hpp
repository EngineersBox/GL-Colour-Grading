#pragma once

#ifndef GL_COLOUR_GRADING_GRADER_HPP
#define GL_COLOUR_GRADING_GRADER_HPP

#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <libconfig.h++>

namespace GLCG {
    class Grader {
        public:
            explicit Grader(const char* filePath);

            void init();
            void destroy();

        [[nodiscard]] GLFWwindow* getWindow() const {
            return this->window;
        }
        [[nodiscard]] constexpr int getWidth() const {
            return this->width;
        }
        [[nodiscard]] constexpr int getHeight() const {
            return this->height;
        }
        [[nodiscard]] constexpr int getOpenGlMajorVersion() const {
            return this->openGlMajorVersion;
        }
        [[nodiscard]] constexpr int getOpenGlMinorVersion() const {
            return this->openGlMinorVersion;
        }
        private:
            libconfig::Config config;
            GLFWwindow* window = nullptr;
            int width = 0;
            int height = 0;
            int openGlMajorVersion = 0;
            int openGlMinorVersion = 0;
            std::string windowTitle;

            void initGLContext();
            void destroyGLContext();
    };
}

#endif //GL_COLOUR_GRADING_GRADER_HPP
