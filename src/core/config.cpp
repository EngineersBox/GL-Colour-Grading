#include "config.hpp"

#include <spdlog/spdlog.h>

namespace GLCG::Core {
    WindowConfig::WindowConfig(libconfig::Config& cfg) {
        if (!cfg.lookupValue("window.width", this->width)) {
            throw std::exception("Unable to read 'window.width' config property");
        } else if (!cfg.lookupValue("window.height", this->height)) {
            throw std::exception("Unable to read 'window.height' config property");
        } else if (!cfg.lookupValue("window.title", this->title)) {
            throw std::exception("Unable to read 'window.title' config property");
        }
    }

    OpenGLConfig::OpenGLConfig(libconfig::Config& cfg) {
        if (!cfg.lookupValue("opengl.major_version", this->majorVersion)) {
            throw std::exception("Unable to read 'opengl.major_version' config property");
        } else if (!cfg.lookupValue("opengl.minor_version", this->minorVersion)) {
            throw std::exception("Unable to read 'opengl.minor_version' config property");
        }
    }

    Config::Config(const char* path) {
        this->cfg.readFile(path);
        spdlog::debug("Loaded config from: {}", path);
        this->window = WindowConfig(this->cfg);
        this->openGL = OpenGLConfig(this->cfg);
    }
}