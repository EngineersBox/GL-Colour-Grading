#include <spdlog/spdlog.h>

#include "core/grader.hpp"
#include "logging/logger.hpp"
#include "gpu/shaders/shader.hpp"
#include "gpu/buffers/vao.hpp"
#include "gpu/buffers/vbo.hpp"
#include "gpu/buffers/ebo.hpp"
#include "gpu/buffers/fbo.hpp"
#include "resources/texture.hpp"
#include "gpu/workgroup.hpp"

// Vertices coordinates
static constexpr GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

// Indices for vertices order
static constexpr GLuint indices[] = {
    0, 2, 1,
    0, 3, 2
};

int main(int argc, const char* argv[]) {
    GLCG::Logger::init();
    GLCG::Core::Grader grader = GLCG::Core::Grader("../assets/config/grader.cfg");
    grader.init();

    GLCG::GPU::WorkGroup wg = GLCG::GPU::WorkGroup();
    spdlog::trace("Work group summary: {}", wg.summary());
    GLCG::GPU::Buffers::FBO fbo = GLCG::GPU::Buffers::FBO(
        grader.getWidth(),
        grader.getHeight()
    );
    GLCG::GPU::Shaders::Shader coreShader = GLCG::GPU::Shaders::Shader::builder()
        .withVertex("../assets/shaders/core.vsh")
        .withFragment("../assets/shaders/core.fsh");
    spdlog::info("Compiled and linked core shader");
    coreShader.validateProgram();
    coreShader.activate();

    GLCG::GPU::Buffers::VAO VAO1 = GLCG::GPU::Buffers::VAO();
    VAO1.bind();

    GLCG::GPU::Buffers::VBO VBO1 = GLCG::GPU::Buffers::VBO(vertices, sizeof(vertices));
    GLCG::GPU::Buffers::EBO EBO1 = GLCG::GPU::Buffers::EBO(indices, sizeof(indices));

    GLCG::GPU::Buffers::VAO::linkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
    GLCG::GPU::Buffers::VAO::linkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    GLCG::GPU::Buffers::VAO::linkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();
    spdlog::info("Created and bound buffers for core shader");

    GLint scaleUniformId = glGetUniformLocation(coreShader.id, "scale");

//    GLCG::GPU::Shaders::Shader core2Shader = GLCG::GPU::Shaders::ShaderBuilder()
//        .markSeparable()
//        .withVertex("../assets/shaders/core.vsh")
//        .withFragment("../assets/shaders/core2.fsh");
//    glCheckError();
//    core2Shader.validateProgram();
//    glCheckError();
//    core2Shader.activate();
//    glCheckError();
//
//    GLCG::GPU::Buffers::VAO VAO2 = GLCG::GPU::Buffers::VAO();
//    VAO2.bind();
//
//    GLCG::GPU::Buffers::VBO VBO2 = GLCG::GPU::Buffers::VBO(vertices, sizeof(vertices));
//    GLCG::GPU::Buffers::EBO EBO2 = GLCG::GPU::Buffers::EBO(indices, sizeof(indices));
//
//    GLCG::GPU::Buffers::VAO::linkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
//    GLCG::GPU::Buffers::VAO::linkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    GLCG::GPU::Buffers::VAO::linkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//
//    VAO2.unbind();
//    VBO2.unbind();
//    EBO2.unbind();
//    spdlog::info("Created and bound buffers for core shader");
//    GLint scaleUniformId2 = glGetUniformLocation(core2Shader.id, "scale");
//    glCheckError();

    const char* imagePath = "../assets/images/golden_gate.png";
    GLCG::Resources::Texture goldenGate = GLCG::Resources::Texture(
        imagePath,
        "diffuse",
        0
    );
    spdlog::info("Loaded image from: {}", imagePath);
    GLCG::Resources::Texture::assignTextureUnit(coreShader, "tex0", 0);

//    GLCG::Resources::Texture::assignTextureUnit(core2Shader, "tex0", 0);
//    glCheckError();
//
//    GLuint pipeline;
//    glGenProgramPipelines(1, &pipeline);
//    glCheckError();
//    glUseProgramStages(
//        pipeline,
//        GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT,
//        coreShader.id
//    );
//    glCheckError();
//    glUseProgramStages(
//        pipeline,
//        GL_FRAGMENT_SHADER_BIT,
//        core2Shader.id
//    );
//    glCheckError();

    spdlog::info("Starting event loop");
    // Main event loop
    while (!glfwWindowShouldClose(grader.getWindow())) {
        fbo.bind();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        coreShader.activate();
        glUniform1f(scaleUniformId, 0.5f);
        goldenGate.bind();
        VAO1.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

//        glUseProgram(0);
//        glBindProgramPipeline(pipeline);
//        glActiveShaderProgram(pipeline, coreShader.id);
//        glUniform1f(scaleUniformId, 0.5f);
//        glCheckError();
//        goldenGate.bind();
//        glCheckError();
//        VAO1.bind();
//        glCheckError();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        glCheckError();
//
//        glActiveShaderProgram(pipeline, core2Shader.id);
//        glCheckError();
//        glUniform1f(scaleUniformId2, 0.5f);
//        glCheckError();
//        goldenGate.bind();
//        glCheckError();
//        VAO2.bind();
//        glCheckError();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        glCheckError();

        fbo.finalise();
        glfwSwapBuffers(grader.getWindow());
        glfwPollEvents();
    }

    spdlog::info("Cleaning up OpenGL resources");
    // Clean up OpenGL resources
    VAO1.destroy();
    VBO1.destroy();
    EBO1.destroy();
//    VAO2.destroy();
//    VBO2.destroy();
//    EBO2.destroy();
    goldenGate.destroy();
    coreShader.destroy();
//    core2Shader.destroy();
    fbo.destroy();

    grader.destroy();

    return 0;
}