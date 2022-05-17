#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "logging/logger.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main(int argc, const char* argv[]) {
    GLColorGrading::Logger::init();
    glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(GLColorGrading::Logger::errorCallbackGLFW));
    glfwInit();
    spdlog::info("Initialised GLFW");

	// Use OpenGL 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Using core profile for only modern APIs
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Set the clear colour for when we invoke glClear on a buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clear the back buffer and set all pixels to the previously set colour
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the buffer context from the front to back buffer
	glfwSwapBuffers(window);

    spdlog::debug("Starting event loop");
	// Main event loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	// Clean up OpenGL resources
	glfwDestroyWindow(window);
	glfwTerminate();
    spdlog::info("Cleaned up GLFW/GLAD/OpenGL resources");

	return 0;
}
