#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#include "logging/logger.hpp"
#include "device/gpu/shaders/shader.hpp"
#include "device/gpu/buffers/vbo.hpp"
#include "device/gpu/buffers/fbo.hpp"
#include "resources/texture.hpp"

static const std::map<std::string, std::array<float, 9>> kernels = {
    { "normal", {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    }},
    { "gaussianBlur", {
        0.045, 0.122, 0.045,
        0.122, 0.332, 0.122,
        0.045, 0.122, 0.045
    }},
    { "gaussianBlur2", {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    }},
    { "gaussianBlur3", {
        0, 1, 0,
        1, 1, 1,
        0, 1, 0
    }},
    { "unsharpen", {
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    }},
    { "sharpness", {
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0
    }},
    { "sharpen", {
        -1, -1, -1,
        -1, 16, -1,
        -1, -1, -1
    }},
    { "edgeDetect", {
        -0.125, -0.125, -0.125,
        -0.125, 1, -0.125,
        -0.125, -0.125, -0.125
    }},
    { "edgeDetect2", {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    }},
    { "edgeDetect3", {
        -5, 0, 0,
        0, 0, 0,
        0, 0, 5
    }},
    { "edgeDetect4", {
        -1, -1, -1,
        0, 0, 0,
        1, 1, 1
    }},
    { "edgeDetect5", {
        -1, -1, -1,
        2, 2, 2,
        -1, -1, -1
    }},
    { "edgeDetect6", {
        -5, -5, -5,
        -5, 39, -5,
        -5, -5, -5
    }},
    { "sobelHorizontal", {
        1, 2, 1,
        0, 0, 0,
        -1, -2, -1
    }},
    { "sobelVertical", {
        1, 0, -1,
        2, 0, -2,
        1, 0, -1
    }},
    { "previtHorizontal", {
        1, 1, 1,
        0, 0, 0,
        -1, -1, -1
    }},
    { "previtVertical", {
        1, 0, -1,
        1, 0, -1,
        1, 0, -1
    }},
    { "boxBlur", {
        0.111, 0.111, 0.111,
        0.111, 0.111, 0.111,
        0.111, 0.111, 0.111
    }},
    { "triangleBlur", {
        0.0625, 0.125, 0.0625,
        0.125, 0.25, 0.125,
        0.0625, 0.125, 0.0625
    }},
    { "emboss", {
        -2, -1, 0,
        -1, 1, 1,
        0, 1, 2
    }}
};

static constexpr GLfloat vertices[] {
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0
};

int main(int argc, const char* argv[]) {
    GLCG::Logger::init();
    GLCG::Device::GPU::Shaders::Shader shader = GLCG::Device::GPU::Shaders::Shader::builder()
        .withVertex("../assets/shaders/post_proc_kernel.vsh")
        .withFragment("../assets/shaders/post_proc_kernel.fsh");
    shader.validateProgram();

    GLint positionLocation = glGetAttribLocation(shader.id, "a_position");
    GLint texCoordLocation=  glGetAttribLocation(shader.id, "a_texCoord");

    GLCG::Device::GPU::Buffers::VBO vbo(
        vertices,
        sizeof(vertices)
    );
    vbo.unbind();

    GLCG::Resources::Texture originalTexture(
        "../assets/images/golden_gate.png",
        "diffuse",
        0
    );

    std::vector<GLCG::Resources::Texture> textures; textures.reserve(2);
    std::vector<GLCG::Device::GPU::Buffers::FBO> framebuffers; framebuffers.reserve(2);
    for (int i = 0; i < 2; i++) {
        textures.push_back(GLCG::Resources::Texture());
        textures[i].resize(
            originalTexture.getWidth(),
            originalTexture.getHeight()
        );
        framebuffers.push_back(GLCG::Device::GPU::Buffers::FBO());
    }

    return 0;
}