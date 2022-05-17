#pragma once

#ifndef GL_COLOUR_GRADING_LOGGER_HPP
#define GL_COLOUR_GRADING_LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/fmt.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <regex>

namespace GLColorGrading::Logger {
    static const std::string DATETIME_LITERAL_FORMAT = "%Y-%m-%d_%H-%M-%S";
    static const std::string LOG_FORMAT = "[{}{}%Y-%m-%d %H:%M:%S{}] [Thread: {}%t{}] [{}%n{}] [{}%^%-5l%${}] :: %v";

    static std::string getCurrentTime(const std::string& date_format) {
        std::time_t rawtime;
        char date_buffer[80];
        std::time(&rawtime);
        struct tm* timeinfo = std::localtime(&rawtime);
        std::strftime(date_buffer, sizeof(date_buffer), date_format.c_str(), timeinfo);
        return {date_buffer};
    }

    static std::string getLogFileName() {
        return "../logs/gl_colour_grading_" + getCurrentTime(DATETIME_LITERAL_FORMAT) + ".log";
    }

    static void generalErrorHandler(const std::string& msg) {
        spdlog::error("[EXCEPTION] :: {0}", msg);
    }

    static void init(spdlog::level::level_enum consoleLevel,
                     spdlog::level::level_enum fileLevel) {
        try {
            std::vector<spdlog::sink_ptr> sinks;

            auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            stdoutSink->set_level(consoleLevel);
            stdoutSink->set_pattern(fmt::format(
                LOG_FORMAT,
                stdoutSink->magenta,
                stdoutSink->bold,
                stdoutSink->reset,
                stdoutSink->blue,
                stdoutSink->reset,
                stdoutSink->cyan,
                stdoutSink->reset,
                stdoutSink->bold,
                stdoutSink->reset
            ));
            sinks.push_back(stdoutSink);

            auto basicFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(getLogFileName());
            basicFileSink->set_level(fileLevel);
            basicFileSink->set_pattern(std::regex_replace(LOG_FORMAT, std::regex("\\{\\}"), ""));
            sinks.push_back(basicFileSink);

            std::shared_ptr<spdlog::logger> combinedLogger = std::make_shared<spdlog::logger>("GL Colour Grading", begin(sinks), end(sinks));
            spdlog::set_default_logger(combinedLogger);
            combinedLogger->set_level(spdlog::level::trace);

            spdlog::set_error_handler(GLColorGrading::Logger::generalErrorHandler);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Logger init failed: " << ex.what() << std::endl;
            exit(1);
        }
//        spdlog::set_pattern(LOG_FORMAT);
    }

    static void init() {
        init(spdlog::level::trace, spdlog::level::trace);
    }

    static void errorCallbackGLFW(int code, const char* message) {
        spdlog::error("GLFW Error ({0}): {1}", code, message);
    }
}

#endif //GL_COLOUR_GRADING_LOGGER_HPP
