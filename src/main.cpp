#include <spdlog/spdlog.h>
#include <memory>

#include "core/grader.hpp"
#include "logging/logger.hpp"
#include "device/gpu/shaders/shader.hpp"
#include "device/gpu/buffers/vao.hpp"
#include "device/gpu/buffers/vbo.hpp"
#include "device/gpu/buffers/ebo.hpp"
#include "device/gpu/buffers/fbo.hpp"
#include "resources/texture.hpp"
#include "device/gpu/workgroup.hpp"
#include "pipeline/rendering/pipelineRenderer.hpp"
#include "device/video/context/ffmpegContext.hpp"

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

static std::vector<GLCG::Pipelines::CoreVertexMeta> passes = std::vector<GLCG::Pipelines::CoreVertexMeta>();

static void bindAttributesAndUniforms(const unsigned int shaderId,
                                      const int resWidth,
                                      const int resHeight,
                                      const int texWidth,
                                      const int texHeight) {
    GLuint positionLocation = glGetAttribLocation(shaderId, "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(
        positionLocation,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
    glVertexAttribPointer(
        glGetAttribLocation(shaderId, "a_texCoord"),
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
    glUniform1f(glGetUniformLocation(shaderId, "u_flipY"), 1);
    glUniform2f(glGetUniformLocation(shaderId, "u_resolution"), static_cast<float>(resWidth), static_cast<float>(resHeight));
    glUniform2f(glGetUniformLocation(shaderId, "u_textureSize"), static_cast<float>(texWidth), static_cast<float>(texHeight));
}

struct Pass1: public GLCG::Pipelines::PipelinePass {
    Pass1(GLCG::Resources::Texture const* texture):
        shader(GLCG::Device::GPU::Shaders::Shader::builder()
            .withVertex("../assets/shaders/post_proc_kernel.vsh")
            .withFragment("../assets/shaders/post_proc_kernel.fsh")),
        vao(GLCG::Device::GPU::Buffers::VAO()),
        texture(texture){}

    // BUG: Fix this bad function call exception
    void operator()(const int width, const int height) {
        this->shader.activate();
        bindAttributesAndUniforms(
            this->shader.id,
            width,
            height,
            this->texture->getWidth(),
            this->texture->getHeight()
        );
        this->vao.bind();
    }

    GLCG::Device::GPU::Shaders::Shader shader;
    GLCG::Device::GPU::Buffers::VAO vao;
    GLCG::Resources::Texture const* texture;
};

struct Pass2: public GLCG::Pipelines::PipelinePass {
    Pass2(GLCG::Resources::Texture const* texture):
        shader(GLCG::Device::GPU::Shaders::Shader::builder()
            .withVertex("../assets/shaders/post_proc_kernel.vsh")
            .withFragment("../assets/shaders/post_proc_kernel.fsh")),
        vao(GLCG::Device::GPU::Buffers::VAO()),
        texture(texture){}

    void operator()(const int width, const int height) {
        this->shader.activate();
        bindAttributesAndUniforms(
            this->shader.id,
            width,
            height,
            this->texture->getWidth(),
            this->texture->getHeight()
        );
        this->vao.bind();
    }

    GLCG::Device::GPU::Shaders::Shader shader;
    GLCG::Device::GPU::Buffers::VAO vao;
    GLCG::Resources::Texture const* texture;
};

GLCG::Pipelines::Pipeline createPipeline(GLCG::Resources::Texture const& texture) {
    GLCG::Pipelines::Pipeline pipeline = GLCG::Pipelines::Pipeline();
    passes.push_back(GLCG::Pipelines::SerialVertex(
        "Vertex 1",
        Pass1(&texture)
    ));
    passes.push_back(GLCG::Pipelines::SerialVertex(
        "Vertex 2",
        Pass2(&texture)
    ));
    GLCG::Pipelines::CoreGraph::Vertex vertex1 = pipeline.addVertex(passes[0]);
    GLCG::Pipelines::CoreGraph::Vertex vertex2 = pipeline.addVertex(passes[1]);
    pipeline.addEdge(vertex1, vertex2);
    return std::move(pipeline);
}

int main(int argc, const char* argv[]) {
    GLCG::Logger::init();
    GLCG::Core::Grader grader = GLCG::Core::Grader("../assets/config/grader.cfg");
    grader.init();

    GLCG::Device::GPU::WorkGroup wg = GLCG::Device::GPU::WorkGroup();
    spdlog::trace("Work group summary: {}", wg.summary());
//    GLCG::Device::GPU::Buffers::FBO fbo = GLCG::Device::GPU::Buffers::FBO(
//        grader.getWidth(),
//        grader.getHeight()
//    );
//    GLCG::Device::GPU::Shaders::Shader coreShader = GLCG::Device::GPU::Shaders::Shader::builder()
//        .withVertex("../assets/shaders/core.vsh")
//        .withFragment("../assets/shaders/core.fsh");
//    spdlog::info("Compiled and linked core shader");
//    coreShader.validateProgram();
//    coreShader.activate();
//
//    GLCG::Device::GPU::Buffers::VAO VAO1 = GLCG::Device::GPU::Buffers::VAO();
//    VAO1.bind();
//
//    GLCG::Device::GPU::Buffers::VBO VBO1 = GLCG::Device::GPU::Buffers::VBO(vertices, sizeof(vertices));
//    GLCG::Device::GPU::Buffers::EBO EBO1 = GLCG::Device::GPU::Buffers::EBO(indices, sizeof(indices));
//
//    GLCG::Device::GPU::Buffers::VAO::linkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
//    GLCG::Device::GPU::Buffers::VAO::linkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    GLCG::Device::GPU::Buffers::VAO::linkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//
//    VAO1.unbind();
//    VBO1.unbind();
//    EBO1.unbind();
//    spdlog::info("Created and bound buffers for core shader");
//
//    GLint scaleUniformId = glGetUniformLocation(coreShader.id, "scale");

    const char* imagePath = "../assets/images/golden_gate.png";
    GLCG::Resources::Texture goldenGate = GLCG::Resources::Texture(
        imagePath,
        "diffuse",
        0
    );
    spdlog::info("Loaded image from: {}", imagePath);
//    GLCG::Resources::Texture::assignTextureUnit(coreShader, "tex0", 0);

    GLCG::Pipelines::Pipeline pipeline = createPipeline(goldenGate);
    GLCG::Pipelines::PipelineRenderer renderer = GLCG::Pipelines::PipelineRenderer(
        std::unique_ptr<GLCG::Pipelines::Pipeline>(&pipeline),
        grader.getWidth(),
        grader.getHeight()
    );
    renderer.registerHandler<GLCG::Pipelines::VertexType::SERIAL>(GLCG::Pipelines::SerialPassHandler());

    spdlog::info("Starting event loop");
    // Main event loop
    while (!glfwWindowShouldClose(grader.getWindow())) {
        // BUG: Fix fbo scaling not matching the window (E.g. using glBindFramebuffer(GL_FRAMEBUFFER, 0) works perfectly)
//        fbo.bind();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
//
//        coreShader.activate();
//        glUniform1f(scaleUniformId, 0.5f);
//        goldenGate.bind();
//        VAO1.bind();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

//        fbo.finalise();


        goldenGate.bind();
        renderer.render();

        glfwSwapBuffers(grader.getWindow());
        glfwPollEvents();
    }

    spdlog::info("Cleaning up OpenGL resources");
    // Clean up OpenGL resources
//    VAO1.destroy();
//    VBO1.destroy();
//    EBO1.destroy();
    goldenGate.destroy();
//    coreShader.destroy();
//    fbo.destroy();

    grader.destroy();

    return 0;
}