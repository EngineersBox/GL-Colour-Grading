#pragma once

#ifndef GL_COLOUR_GRADING_STRINGUTILS_HPP
#define GL_COLOUR_GRADING_STRINGUTILS_HPP

#include <string>
#include <memory>
#include <stdexcept>

namespace GLCG::Utils::String {
    template<typename ...T>
    std::string format(const std::string& format, T... args) {
        int size_s = std::snprintf(
            nullptr,
            0,
            format.c_str(),
            args...
        ) + 1;
        if (size_s <= 0) {
            throw std::runtime_error("Error during formatting.");
        }
        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(
            buf.get(),
            size,
            format.c_str(),
            args...
        );
        return std::string(buf.get(), buf.get() + size - 1);
    }
}

#endif //GL_COLOUR_GRADING_STRINGUTILS_HPP
