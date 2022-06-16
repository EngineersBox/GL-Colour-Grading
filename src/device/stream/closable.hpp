#pragma once

#ifndef GL_COLOUR_GRADING_CLOSABLE_HPP
#define GL_COLOUR_GRADING_CLOSABLE_HPP

namespace GLCG::Device::Stream {
    enum class ClosableResourceState {
        OPEN,
        CLOSED
    };
    struct Closable {
        virtual void open() {
            this->resourceState = ClosableResourceState::OPEN;
        }
        virtual void close() {
            this->resourceState = ClosableResourceState::CLOSED;
        }
        ClosableResourceState resourceState;
    };
}

#endif //GL_COLOUR_GRADING_CLOSABLE_HPP
