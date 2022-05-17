#pragma once

#ifndef GL_COLOUR_GRADING_GPUBUFFER_HPP
#define GL_COLOUR_GRADING_GPUBUFFER_HPP

namespace GLCG::GPU::Buffers {
    class GpuBuffer {
        public:
            GpuBuffer() = default;

            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual void destroy() = 0;
    };
}

#endif //GL_COLOUR_GRADING_GPUBUFFER_HPP
