#pragma once

#include "client.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

class GLFWCallbackHandler {
public:
	static void add(GLFWwindow *, Client *);
	static void remove(GLFWwindow *, Client *);
	static std::unordered_map<GLFWwindow *, Client *> clients;
};