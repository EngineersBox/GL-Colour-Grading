#pragma once

#ifndef GL_COLOUR_GRADING_OPENGLFEATURESUPPORTEXCEPTION_HPP
#define GL_COLOUR_GRADING_OPENGLFEATURESUPPORTEXCEPTION_HPP

#include <stdexcept>
#include "../../../util/stringUtils.hpp"

namespace GLCG::GPU::Shaders {
    class OpenGLFeatureSupportException : public std::runtime_error {
        public:
            OpenGLFeatureSupportException(const char* feature,
                                          const int majorVersion,
                                          const int minorVersion):
                std::runtime_error(GLCG::Utils::String::format(
                    "Unsupported OpenGL feature \"%s\", requires minimum version %d.%d",
                    feature,
                    majorVersion,
                    minorVersion
                ))
            {}
    };
}

#endif //GL_COLOUR_GRADING_OPENGLFEATURESUPPORTEXCEPTION_HPP
