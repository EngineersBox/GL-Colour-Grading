#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "logging/logger.hpp"
#include "gpu/shaders/shader.hpp"
#include "gpu/buffers/vao.hpp"
#include "gpu/buffers/vbo.hpp"
#include "gpu/buffers/ebo.hpp"
#include "gpu/buffers/fbo.hpp"
#include "resources/texture.hpp"

static constexpr int WINDOW_WIDTH = 1920;
static constexpr int WINDOW_HEIGHT = 1080;

// Vertices coordinates
static constexpr GLfloat vertices[] = {
    //     COORDINATES              /        COLORS                /   TexCoord  //
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Lower left corner
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Upper left corner
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
static constexpr GLuint indices[] = {
    0, 2, 1, // Upper triangle
    0, 3, 2 // Lower triangle
};

int main(int argc, const char* argv[]) {
    GLCG::Logger::init();
    glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(GLCG::Logger::errorCallbackGLFW));
    glfwInit();
    spdlog::debug("Initialised GLFW");

	// Use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Using core profile for only modern APIs
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"GL Rendering",
		nullptr,
		nullptr
	);
	if (window == nullptr) {
        spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		return 1;
	}
	// Contextualise the current window to draw to
	glfwMakeContextCurrent(window);

	gladLoadGL();
    spdlog::trace("OpenGL Version: {}", glGetString(GL_VERSION));
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLCG::GPU::Buffers::FBO fbo = GLCG::GPU::Buffers::FBO(
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
    GLCG::GPU::Shaders::Shader coreShader = GLCG::GPU::Shaders::Shader(
        "../assets/shaders/core.vsh",
        "../assets/shaders/core.fsh"
    );
    spdlog::info("Compiled and linked core core");

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
    spdlog::info("Created and bound buffers for coreShader");

    GLuint uniformId = glGetUniformLocation(coreShader.id, "scale");

    const char* imagePath = "../assets/images/golden_gate.png";
    GLCG::Resources::Texture goldenGate = GLCG::Resources::Texture(
        imagePath,
        "diffuse",
        GL_TEXTURE0
    );
    spdlog::debug("Loaded image from: {}", imagePath);
    GLCG::Resources::Texture::assignTextureUnit(coreShader, "tex0", 0);

    spdlog::debug("Starting event loop");
	// Main event loop
	while (!glfwWindowShouldClose(window)) {
        fbo.bind();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        coreShader.activate();
        glUniform1f(uniformId, 0.5f);
        goldenGate.bind();
        VAO1.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        fbo.finalise();
        glfwSwapBuffers(window);
        glfwPollEvents();
	}

	// Clean up OpenGL resources
    VAO1.destroy();
    VBO1.destroy();
    EBO1.destroy();
    goldenGate.destroy();
    coreShader.destroy();
    fbo.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
    spdlog::debug("Cleaned up GLFW/GLAD/OpenGL resources");

	return 0;
}
