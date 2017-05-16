#include "glfw_callback_handler.h"
#include "logging.h"

std::unordered_map<GLFWwindow *, Client *> GLFWCallbackHandler::clients;

void errorCallback(int error, const char * description) {
	LOG_ERR(description);
}
void resizeCallback(GLFWwindow * window, int width, int height) {
	if (GLFWCallbackHandler::clients[window])
		GLFWCallbackHandler::clients[window]->resizeCallback(width, height);
}
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (GLFWCallbackHandler::clients[window])
		GLFWCallbackHandler::clients[window]->keyboard_handler.keyCallback(key, scancode, action, mods);
}
void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
	if (GLFWCallbackHandler::clients[window])
		GLFWCallbackHandler::clients[window]->mouse_handler.mouseButtonCallback(button, action, mods);
}
void mouseCursorCallback(GLFWwindow * window, double x, double y) {
	if (GLFWCallbackHandler::clients[window])
		GLFWCallbackHandler::clients[window]->mouse_handler.mouseCursorCallback(x, y);
}
void mouseWheelCallback(GLFWwindow * window, double x, double y) {
	if (GLFWCallbackHandler::clients[window])
		GLFWCallbackHandler::clients[window]->mouse_handler.mouseWheelCallback(x, y);
}
void GLFWCallbackHandler::add(GLFWwindow * window, Client * client) {
	glfwSetErrorCallback(errorCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseCursorCallback);
	glfwSetScrollCallback(window, mouseWheelCallback);

	clients[window] = client;
}

void GLFWCallbackHandler::remove(GLFWwindow * window, Client * client) {
	if (clients[window] != client)
		return;
	else
		clients.erase(window);
}
