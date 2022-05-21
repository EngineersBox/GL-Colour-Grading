#pragma once

#ifndef GL_COLOUR_GRADING_WORKGROUP_HPP
#define GL_COLOUR_GRADING_WORKGROUP_HPP

#include <glm/vec3.hpp>

namespace GLCG::GPU {
    using WorkGroup = struct WorkGroup {
        WorkGroup();

        glm::ivec3 count;
        glm::ivec3 size;
        int invocations = 0;
    };
}

#endif //GL_COLOUR_GRADING_WORKGROUP_HPP
