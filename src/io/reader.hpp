#pragma once

#ifndef GL_COLOUR_GRADING_READER_HPP
#define GL_COLOUR_GRADING_READER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

namespace GLCG::Reader {

    static std::string readFileIntoString(const std::string& path) {
        std::ifstream input_file(path);
        if (!input_file.is_open()) {
            spdlog::error("Could not open file: {}", path);
            exit(1);
        }
        return {(std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>()};
    }

}

#endif //GL_COLOUR_GRADING_READER_HPP
