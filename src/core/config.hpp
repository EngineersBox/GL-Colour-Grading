#pragma once

#ifndef GL_COLOUR_GRADING_CONFIG_HPP
#define GL_COLOUR_GRADING_CONFIG_HPP

#include <string>
#include <libconfig.h++>

namespace GLCG::Core {
    using WindowConfig = struct WindowConfig {
        WindowConfig() = default;
        explicit WindowConfig(libconfig::Config& cfg);

        int width = 0;
        int height = 0;
        std::string title;
    };
    using OpenGLConfig = struct OpenGLConfig {
        OpenGLConfig() = default;
        explicit OpenGLConfig(libconfig::Config& cfg);

        int majorVersion = 0;
        int minorVersion = 0;
    };
    using Config = struct Config {
        Config() = default;
        explicit Config(const char* path);

        WindowConfig window;
        OpenGLConfig openGL;
        libconfig::Config cfg;
    };
}

#endif //GL_COLOUR_GRADING_CONFIG_HPP
