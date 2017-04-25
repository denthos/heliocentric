#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "client.h"
#include "logging.h"
#include "ini_parser.h"

int main() {
	int width = Lib::INIParser::getInstance("config.ini").get<int>("ScreenWidth");
	int height = Lib::INIParser::getInstance("config.ini").get<int>("ScreenHeight");

	GLFWwindow * window = Client::createWindow(width, height);

	if (!window) {
		return 1;
	}


	Lib::LOG_DEBUG("Supported OpenGL version: ", glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
	Lib::LOG_DEBUG("Supported GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
	Lib::LOG_DEBUG("Glew version: ", glewGetString(GLEW_VERSION));
	Lib::LOG_DEBUG("Renderer: ", glGetString(GL_RENDERER));

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