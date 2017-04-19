#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "client.h"

int main() {

	GLFWwindow * window = Client::createWindow(1366, 768);

	if (!window) {
		return 1;
	}


	fprintf(stdout, "Supported OpenGL version: %s\n", glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
	fprintf(stdout, "Supported GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
	fprintf(stdout, "Glew version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));

	// Set up callback functions
	glfwSetErrorCallback(Client::errorCallback);
	glfwSetFramebufferSizeCallback(window, Client::resizeCallback);
	glfwSetKeyCallback(window, Client::keyCallback);
	glfwSetMouseButtonCallback(window, Client::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Client::mouseCursorCallback);
	glfwSetScrollCallback(window, Client::mouseWheelCallback);

	Client::initialize();

	while (!glfwWindowShouldClose(window)) {
		Client::display(window);
		Client::update();
	}

	glfwTerminate();

	Client::clean();
	return 0;
}