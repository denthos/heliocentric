#include "client.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <soil.h>


#include "Planet.h"
#include "sphere_mesh.h"
#include "transformation.h"
#include "orbit.h"

#include "model.h"


#define WINDOW_TITLE "Heliocentric"
#define VERT_SHADER "Shaders/shader.vert"
#define FRAG_SHADER "Shaders/shader.frag"

#define TEXTURE_VERT_SHADER "Shaders/simple_texture.vert"
#define TEXTURE_FRAG_SHADER "Shaders/simple_texture.frag"

#define EARTH_TEXTURE "Textures/earth.jpg"
#define SUN_TEXTURE "Textures/sun.jpg"

#define ROCKET_MODEL "../models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"

Model rocket;
PlanetModel * earth;
PlanetModel  * sun;

Shader* shader; //TODO reimplement so it doesn't need to be a pointer on heap?
Shader* textureShader;
//don't forget to clean up afterwards

#include "game_channels.h"

int Client::width, Client::height;
float Client::fov = 45.0f, Client::nearPlane = 0.5f, Client::farPlane = 10000.0f;
glm::mat4 Client::perspectiveMatrix, Client::viewMatrix;
glm::vec3 Client::camPos(0.0f, 0.f, 20.0f), Client::camLookAt(0.0f, 0.0f, 0.0f), Client::camUp(0.0f, 1.0f, 0.0f);
std::string Client::windowTitle(WINDOW_TITLE);

std::unordered_map<UID, Player *> Client::playerMap;
std::unordered_map<UID, Planet *> Client::planetMap;
std::unordered_map<UID, Unit *> Client::unitMap;
std::unordered_map<UID, City *> Client::cityMap;
std::unordered_map<UID, Slot *> Client::slotMap;
//Octree<GameObject *> Client::octree;


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

	glShadeModel(GL_SMOOTH);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	viewMatrix = glm::lookAt(camPos, camLookAt, camUp);



	shader = new Shader(VERT_SHADER, FRAG_SHADER);
	textureShader = new Shader(TEXTURE_VERT_SHADER, TEXTURE_FRAG_SHADER);
	
	rocket = Model(ROCKET_MODEL);
	//rocket.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
	earth = new PlanetModel(Texture(EARTH_TEXTURE), 5.0f, Orbit(50.0f, 0.06f));
	sun = new PlanetModel(Texture(SUN_TEXTURE), 15.0f, Orbit(0.0f, 0.0f));
	
	// Set up SunNet client and channel callbacks
	initializeChannels();

	// TODO
}

void Client::display(GLFWwindow * window) {
	// Set view matrix
	viewMatrix = glm::lookAt(camPos, camLookAt, camUp);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	
	

	
	earth->Draw(*textureShader);
	rocket.Draw(*textureShader);
	sun->Draw(*textureShader);

	glfwSwapBuffers(window);

	glfwPollEvents();
}

void Client::update() {
	sun->Update(glm::mat4(1.0f));
	earth->Update(glm::mat4(1.0f));
	rocket.Update(glm::mat4(1.0f));
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
		case(GLFW_KEY_ESCAPE):
			break;
		default:
			break;
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


void Client::clean()
{
	delete shader;
	delete textureShader;

	delete earth;
	delete sun;
}

void Client::playerUpdateHandler(PlayerUpdate * update) {
	update->apply(*playerMap[update->id]);
}

void Client::unitUpdateHandler(UnitUpdate * update) {
	update->apply(*unitMap[update->id]);
	// update octree
}

void Client::cityUpdateHandler(CityUpdate * update) {
	update->apply(*cityMap[update->id]);
	// update octree
}

void Client::planetUpdateHandler(PlanetUpdate * update) {
	update->apply(*planetMap[update->id]);
	// update octree
}

void Client::slotUpdateHandler(SlotUpdate * update) {
	update->apply(*slotMap[update->id]);
	// update octree
}

