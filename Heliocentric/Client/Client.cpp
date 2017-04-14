#include "Client.h"

#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <soil.h>

#include "ShaderUtils.h"
#include "Sphere.h"

int Client::width, Client::height;
float Client::fov = 45.0f, Client::nearPlane = 0.5f, Client::farPlane = 10000.0f;
glm::mat4 Client::perspectiveMatrix, Client::viewMatrix;
glm::vec3 Client::camPos(0.0f, 0.f, 20.0f), Client::camLookAt(0.0f, 0.0f, 0.0f), Client::camUp(0.0f, 1.0f, 0.0f);
std::string Client::windowTitle("Heliocentric");


GLuint defaultShader;

std::pair<double, double> lastMousePos;
bool leftMouseDown = false;
bool rightMouseDown = false;
bool middleMouseDown = false;

#define FREE_CAMERA 1
#define FREE_CAMERA_2 2
#define ORBITAL_CAMERA 3
unsigned char selectedControlScheme = FREE_CAMERA;

GLFWwindow * Client::createWindow(int width, int height) {

	if (!glfwInit()) {
		fprintf(stderr, "Could not initialize GLFW\n");
		return NULL;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow * window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	if (!window) {
		fprintf(stderr, "Could not create GLFW window\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);

	GLenum glewErr = glewInit();
#ifndef __APPLE__
	if (glewErr != GLEW_OK) {
		fprintf(stderr, "Glew failed to initialize: %s\n", glewGetErrorString(glewErr));
		glfwTerminate();
		return NULL;
	}
#endif

	Client::resizeCallback(window, width, height);

	return window;
}

void Client::initialize() {

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

	viewMatrix = glm::lookAt(camPos, camLookAt, camUp);

	compileShaders("Shaders/shader.vert", "Shaders/shader.frag", &defaultShader);

	// load earth texture
	int earthTextureWidth, earthTextureHeight;
	unsigned char * earthTextureImage = SOIL_load_image("Textures/earth.jpg", &earthTextureWidth, &earthTextureHeight, 0, SOIL_LOAD_RGB);
	GLuint earthTexture;
	glGenTextures(1, &earthTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, earthTextureWidth, earthTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthTextureImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	// free earth texture resources
	SOIL_free_image_data(earthTextureImage);

	// load sun texture
	int sunTextureWidth, sunTextureHeight;
	unsigned char * sunTextureImage = SOIL_load_image("Textures/sun.jpg", &sunTextureWidth, &sunTextureHeight, 0, SOIL_LOAD_RGB);
	GLuint sunTexture;
	glGenTextures(1, &sunTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sunTextureWidth, sunTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sunTextureImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	// free sun texture resources
	SOIL_free_image_data(sunTextureImage);
}

void Client::display(GLFWwindow * window) {
	// Set view matrix
	viewMatrix = glm::lookAt(camPos, camLookAt, camUp);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glfwSwapBuffers(window);

	glfwPollEvents();
}

void Client::update() {

}

void Client::errorCallback(int error, const char * description) {
	fprintf(stderr, "OpenGL error occurred: %s", description);
}

void Client::resizeCallback(GLFWwindow * window, int width, int height) {
	
	Client::width = width;
	Client::height = height;

	glViewport(0, 0, width, height);

	if (height > 0) {
		Client::perspectiveMatrix = glm::perspective(fov, (float)width / (float)height, nearPlane, farPlane);
	}

}

void Client::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case(GLFW_KEY_ESCAPE):
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {

		}
	}
}

void Client::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		lastMousePos = std::make_pair(x, y);
		switch (button) {
		case(GLFW_MOUSE_BUTTON_LEFT):
			leftMouseDown = true;
			break;
		case(GLFW_MOUSE_BUTTON_RIGHT):
			rightMouseDown = true;
			break;
		case(GLFW_MOUSE_BUTTON_MIDDLE):
			middleMouseDown = true;
			break;
		default:
			return;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case(GLFW_MOUSE_BUTTON_LEFT):
			leftMouseDown = false;
			break;
		case(GLFW_MOUSE_BUTTON_RIGHT):
			rightMouseDown = false;
			break;
		case(GLFW_MOUSE_BUTTON_MIDDLE):
			middleMouseDown = false;
			break;
		default:
			return;
		}
	}
}

void Client::mouseCursorCallback(GLFWwindow * window, double x, double y) {

	if (leftMouseDown) {
		// move selected control point
		glm::vec3 deltaY = glm::normalize(camUp) * (float)(lastMousePos.second - y) / 100.0f;
		glm::vec3 deltaX = glm::normalize(glm::cross(camPos, camUp)) * (float)(lastMousePos.first - x) / 100.0f;
		glm::vec3 delta = deltaY + deltaX;


		lastMousePos = std::make_pair(x, y);
	}
	if (rightMouseDown) {
		float angle;
		// Perform horizontal (y-axis) rotation
		angle = (float)(lastMousePos.first - x) / 100.0f;
		camPos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camPos, 1.0f));
		camUp = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camUp, 1.0f));
		//Now rotate vertically based on current orientation
		angle = (float)(y - lastMousePos.second) / 100.0f;
		glm::vec3 axis = glm::cross((camPos - camLookAt), camUp);
		camPos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(camPos, 1.0f));
		camUp = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(camUp, 1.0f));
		lastMousePos = std::make_pair(x, y);
	}
}

void Client::mouseWheelCallback(GLFWwindow * window, double x, double y) {
	camPos = glm::vec3(glm::translate(glm::mat4(1.0f), camPos * (float)y * -0.05f) * glm::vec4(camPos, 1.0f));
}
