cmake_minimum_required(VERSION 3.10)

project(GL_Colour_Grading VERSION 1.0)

# Debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1")

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CXXFLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include source content
file(GLOB_RECURSE GL_Colour_Grading_SOURCES CONFIGURE_DEPENDS "src/**.cpp")
file(GLOB_RECURSE GL_Colour_Grading_HEADERS CONFIGURE_DEPENDS "src/**.hpp")

set(GL_Colour_Grading_INCLUDE_DIRS "")
foreach(_headerFile ${GL_Colour_Grading_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND GL_Colour_Grading_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES GL_Colour_Grading_INCLUDE_DIRS)

# Mark executable
add_executable(GL_Colour_Grading ${GL_Colour_Grading_SOURCES})
target_include_directories(GL_Colour_Grading PRIVATE ${GL_Colour_Grading_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(GL_Colour_Grading PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(GL_Colour_Grading PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(GL_Colour_Grading PUBLIC ${OPENGL_LIBRARIES})

# OpenCL
find_package(OpenCL REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE OpenCL::OpenCL)

# GLFW 3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE glfw)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE GLEW::GLEW)

# GLAD
find_package(glad CONFIG REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE glad::glad)

# GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE glm::glm)

# spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(GL_Colour_Grading PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

# Boost
find_package(Boost REQUIRED COMPONENTS filesystem iostreams graph)
target_link_libraries(GL_Colour_Grading PRIVATE Boost::filesystem Boost::iostreams)

# stb
find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(GL_Colour_Grading PRIVATE ${STB_INCLUDE_DIRS})

# libconfig
find_library(LIBCONFIGPP_LOCATION libconfig++)
find_library(LIBCONFIG_LOCATION libconfig)
target_link_libraries(GL_Colour_Grading PRIVATE ${LIBCONFIGPP_LOCATION} ${LIBCONFIG_LOCATION})

# FFMPEG Libs
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG_LIBS REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
    x264
    x265
    ffnvcodec
)
target_link_libraries(GL_Colour_Grading PRIVATE PkgConfig::FFMPEG_LIBS)

# FFMPEG Libs
#set(
#    FFMPEG_PATHS
#    "libopengl/opengl.h"
#    "libopencl/opencl.h"
#    "libnvcodec/nvcodec.h"
#    "libx264/x264.h"
#    "libx265/x265.h"
#)
#set(
#    FFMPEG_LIBS
#    "opengl"
#    "opencl"
#    "nvcodec"
#    "x264"
#    "x265"
#)
#set(FFMPEG_INCLUDE_DIRECTORIES "")
#set(FFMPEG_LINK_LIBRARIES "")
#foreach(_ffmpeg_path ${FFMPEG_PATHS})
#    set(FFMPEG_INCLUDE "")
#    find_path(FFMPEG_INCLUDE _ffmpeg_path)
#    list(APPEND FFMPEG_INCLUDE_DIRECTORIES ${FFMPEG_INCLUDE})
#endforeach()
#foreach(_ffmpeg_lib ${FFMPEG_LIBS})
#    set(FFMPEG_LIBRARY "")
#    find_path(FFMPEG_LIBRARY _ffmpeg_lib)
#    list(APPEND FFMPEG_LINK_LIBRARIES ${FFMPEG_LIBRARY})
#endforeach()
#message("Include directories: ${FFMPEG_INCLUDE_DIRECTORIES}")
#target_include_directories(GL_Colour_Grading PRIVATE ${FFMPEG_INCLUDE_DIRECTORIES})
#message("Link libraries: ${FFMPEG_LINK_LIBRARIES}")
#target_link_libraries(GL_Colour_Grading PRIVATE ${FFMPEG_LINK_LIBRARIES})