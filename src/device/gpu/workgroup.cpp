#include "workgroup.hpp"

#include <glad/glad.h>
#include "../util/stringUtils.hpp"
#include "../util/openglUtils.hpp"

namespace GLCG::Device::GPU {
    WorkGroup::WorkGroup() {
        if (!Utils::OpenGL::isVersionSupported(4, 3)) {
            // Compute shaders are not supported in versions older than 4.3
            return;
        }

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &this->count.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &this->count.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &this->count.z);

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &this->size.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &this->size.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &this->size.z);

        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &this->invocations);

        glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &this->sharedMemorySizeBytes);
    }

    std::string WorkGroup::summary() {
        return GLCG::Utils::String::format(
            "Count: {X=%d Y=%d Z=%d} Size: {X=%d Y=%d Z=%d} Invocations: %d Shared Memory: %d",
            this->count.x,
            this->count.y,
            this->count.z,
            this->size.x,
            this->size.y,
            this->size.z,
            this->invocations,
            this->sharedMemorySizeBytes
        );
    }
}