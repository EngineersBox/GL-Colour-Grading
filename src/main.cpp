#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "logging/logger.hpp"
#include "gpu/shaders/shader.hpp"
#include "gpu/buffers/vao.hpp"
#include "gpu/buffers/vbo.hpp"
#include "gpu/buffers/ebo.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

static constexpr char VERTEX_FILE_PATH[] = "../assets/shaders/triangle.vsh";
static constexpr char FRAGMENT_FILE_PATH[] = "../assets/shaders/triangle.fsh";

GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
    0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
    0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
    -0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
    0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
    0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
};

// Indices for vertices order
GLuint indices[] = {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1 // Upper triangle
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

    GLCG::GPU::Shaders::Shader shader = GLCG::GPU::Shaders::Shader(
        VERTEX_FILE_PATH,
        FRAGMENT_FILE_PATH
    );
    spdlog::info("Compiled and linked shader");

    GLCG::GPU::Buffers::VAO VAO1 = GLCG::GPU::Buffers::VAO();
    VAO1.bind();

    GLCG::GPU::Buffers::VBO VBO1 = GLCG::GPU::Buffers::VBO(vertices, sizeof(vertices));
    GLCG::GPU::Buffers::EBO EBO1 = GLCG::GPU::Buffers::EBO(indices, sizeof(indices));

    VAO1.linkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), nullptr);
    VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();
    spdlog::info("Created and bound buffers for shader");

    GLuint uniformId = glGetUniformLocation(shader.id, "scale");

    spdlog::debug("Starting event loop");
	// Main event loop
	while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.activate();
        glUniform1f(uniformId, 0.5f);
        VAO1.bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
	}

	// Clean up OpenGL resources
    VAO1.destroy();
    VBO1.destroy();
    EBO1.destroy();
    shader.remove();
	glfwDestroyWindow(window);
	glfwTerminate();
    spdlog::debug("Cleaned up GLFW/GLAD/OpenGL resources");

	return 0;
}
