#ifdef _WIN32
	#define APIENTRY __stdcall
#endif

#include <glad\glad.h>

#ifdef _WINDOWS_
	#error windows.h was included
#endif

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unordered_map>

#include "client.h"
#include "logging.h"
#include "ini_parser.h"

int main() {
	Client client;

	LOG_DEBUG("Supported OpenGL version: ", glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
	LOG_DEBUG("Supported GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
	LOG_DEBUG("Renderer: ", glGetString(GL_RENDERER));

	// Set up callback functions

	while (client.isRunning()) {
		client.display();
		client.update();
	}

	client.disconnect();
	glfwTerminate();
	return 0;
}

