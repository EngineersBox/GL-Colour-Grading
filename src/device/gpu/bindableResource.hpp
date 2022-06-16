#pragma once

#ifndef GL_COLOUR_GRADING_BINDABLERESOURCE_HPP
#define GL_COLOUR_GRADING_BINDABLERESOURCE_HPP

namespace GLCG::Device::GPU {
    class BindableResource {
        public:
            BindableResource() = default;

            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual void destroy() = 0;
    };
}

#endif //GL_COLOUR_GRADING_BINDABLERESOURCE_HPP
