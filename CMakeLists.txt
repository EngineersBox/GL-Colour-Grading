cmake_minimum_required(VERSION 3.10)

project(GL_Colour_Grading VERSION 1.0)

# Debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1")

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CXXFLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include source content
file (GLOB_RECURSE GL_Colour_Grading_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE GL_Colour_Grading_HEADERS CONFIGURE_DEPENDS "src/*.h")

set (GL_Colour_Grading_INCLUDE_DIRS "")
foreach (_headerFile ${GL_Colour_Grading_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND GL_Colour_Grading_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES GL_Colour_Grading_INCLUDE_DIRS)

# Mark executable
add_executable(GL_Colour_Grading ${GL_Colour_Grading_SOURCES})
target_include_directories(GL_Colour_Grading PRIVATE ${GL_Colour_Grading_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(GL_Colour_Grading PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(GL_Colour_Grading PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(GL_Colour_Grading PUBLIC ${OPENGL_LIBRARIES})

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

# boost
find_package(Boost REQUIRED filesystem iostreams COMPONENTS)
target_link_libraries(GL_Colour_Grading PRIVATE Boost::filesystem Boost::iostreams)

# stb
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(GL_Colour_Grading PRIVATE ${STB_INCLUDE_DIRS})

