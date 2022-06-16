#pragma once

#ifndef GL_COLOUR_GRADING_WORKGROUP_HPP
#define GL_COLOUR_GRADING_WORKGROUP_HPP

#include <glm/vec3.hpp>
#include <string>

namespace GLCG::Device::GPU {
    using WorkGroup = struct WorkGroup {
        WorkGroup();

        std::string summary();

        glm::ivec3 count = glm::ivec3(0,0,0);
        glm::ivec3 size = glm::ivec3(0,0,0);
        int invocations = 0;
        int sharedMemorySizeBytes = 0;
    };
}

#endif //GL_COLOUR_GRADING_WORKGROUP_HPP
