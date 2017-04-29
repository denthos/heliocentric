#include "client.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <stdio.h>
#include <soil.h>



#include "planet.h"
#include "skybox_mesh.h"
#include "glfw_callback_handler.h"
#include "game_channels.h"
#include "sphere_mesh.h"
#include "transformation.h"
#include "orbit.h"
#include "model.h"
#include "logging.h"

#include "debug_pause.h"
#include "unit_create_command.h"



#define WINDOW_TITLE "Heliocentric"
#define DEFAULT_WIDTH 1366
#define DEFAULT_HEIGHT 768
#define VERT_SHADER "Shaders/shader.vert"
#define FRAG_SHADER "Shaders/shader.frag"
#define CUBEMAP_VERT_SHADER "Shaders/cubemap.vert"
#define CUBEMAP_FRAG_SHADER "Shaders/cubemap.frag"
#define TEXTURE_VERT_SHADER "Shaders/simple_texture.vert"
#define TEXTURE_FRAG_SHADER "Shaders/simple_texture.frag"

#define EARTH_TEXTURE "Textures/earth.jpg"
#define SUN_TEXTURE "Textures/sun.jpg"

#define ROCKET_MODEL "../models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"

//skybox texture files
#define SKYBOX_FRONT "Textures/Skybox/front.png" 
#define SKYBOX_BACK "Textures/Skybox/back.png"
#define SKYBOX_TOP "Textures/Skybox/top.png"
#define SKYBOX_BOTTOM "Textures/Skybox/bottom.png"
#define SKYBOX_LEFT "Textures/Skybox/left.png"
#define SKYBOX_RIGHT "Textures/Skybox/right.png"

Model rocket;

SkyboxMesh* skybox;


Shader* shader; //TODO reimplement so it doesn't need to be a pointer on heap?
Shader* textureShader;
Shader* cubemapShader;
//don't forget to clean up afterwards

GLuint defaultShader;

std::pair<double, double> lastMousePos;
bool leftMouseDown = false;
bool rightMouseDown = false;
bool middleMouseDown = false;

#define FREE_CAMERA 1
#define FREE_CAMERA_2 2
#define ORBITAL_CAMERA 3
unsigned char selectedControlScheme = FREE_CAMERA;

Client::Client() : SunNet::ChanneledClient<SunNet::TCPSocketConnection>(10/*TODO config*/) {
	// TODO: Replace these hardcoded constants with values read from config file
	windowTitle = WINDOW_TITLE;
	createWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);

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

	camera = new Camera(glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 1.0f, 10000.0f, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	resizeCallback(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	shader = new Shader(VERT_SHADER, FRAG_SHADER);
	textureShader = new Shader(TEXTURE_VERT_SHADER, TEXTURE_FRAG_SHADER);
	cubemapShader = new Shader(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	
	//rocket = Model(ROCKET_MODEL);
	//rocket.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));

	skybox = new SkyboxMesh(SKYBOX_RIGHT, SKYBOX_LEFT, SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_BACK, SKYBOX_FRONT);
	//TODO rewrite this
	skybox->world_mat =  glm::scale(skybox->world_mat, glm::vec3(4000.0f));

	for (auto& planet : this->universe.get_planets()) {
		auto earth_model = new PlanetModel(Texture(EARTH_TEXTURE), planet->get_radius(), Orbit(0.0f, 0.0f));
		this->planetMap[planet->getID()] = std::make_pair(planet.get(), earth_model);
	}

	// Set up SunNet client and channel callbacks
	initializeChannels();

	this->subscribe<PlayerUpdate>(std::bind(&Client::playerUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitUpdate>(std::bind(&Client::unitUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityUpdate>(std::bind(&Client::cityUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlanetUpdate>(std::bind(&Client::planetUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<SlotUpdate>(std::bind(&Client::slotUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));

	std::string address = Lib::INIParser::getInstance().get<std::string>("ServerHost");
	std::string port = Lib::INIParser::getInstance().get<std::string>("ServerPort");
	try {
		this->connect(address, port);
	}
	catch (const SunNet::ConnectException&) {
		Lib::LOG_ERR("Could not connect to host at address ", address, " and port ", port);
	}
	//this->connect(std::string address, std::string port);
	//this->channeled_send<UnitUpdate>(UnitUpdate*);

	// TODO
}

Client::~Client() {
	delete shader;
	delete textureShader;

	for (auto& planet_item : planetMap) {
		delete planet_item.second.second;
	}
	GLFWCallbackHandler::remove(window, this);
}

bool Client::isRunning() {
	return !glfwWindowShouldClose(window);
}

void Client::createWindow(int width, int height) {
	if (!glfwInit()) {
		Lib::LOG_ERR("Could not initialize GLFW");
		return;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	if (!window) {
		Lib::LOG_ERR("Could not create GLFW window");
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);

	GLenum glewErr = glewInit();
#ifndef __APPLE__
	if (glewErr != GLEW_OK) {
		Lib::LOG_ERR("Glew failed to initialize: ", glewGetErrorString(glewErr));
		return;
	}
#endif

	resizeCallback(width, height);

	GLFWCallbackHandler::add(window, this);

}

void Client::display() {
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	camera->calculateViewMatrix();

	for (auto& planet_pair : this->planetMap) {
		planet_pair.second.second->Draw(*textureShader, *camera);
	}
	
	//rocket.Draw(*textureShader, *camera);


	octree.viewFrustumCull(ViewFrustum()); // TODO: get view frustum from camera
	//octree.draw(textureShader->getPid(), *camera);
	octree.clear(); // empty octree each frame

	skybox->Draw(*cubemapShader, *camera);

	glfwSwapBuffers(window);

	glfwPollEvents();
}

void Client::update() {
	for (auto& planet_pair : this->planetMap) {
		planet_pair.second.second->Update(glm::translate(glm::mat4(1.0f), planet_pair.second.first->get_position()));
	}

	rocket.Update(glm::mat4(1.0f));
}

void Client::errorCallback(int error, const char * description) {
	Lib::LOG_ERR("OpenGL Error occurred: ", description);
}

void Client::resizeCallback(int width, int height) {
	if (camera) {
		camera->width = width;
		camera->height = height;
		camera->calculatePerspectiveMatrix();
	}

	glViewport(0, 0, width, height);
}

void Client::keyCallback(int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case(GLFW_KEY_ESCAPE):
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case(GLFW_KEY_F1):
			/* Toggle the server's pause state */
			DebugPause pause;
			this->channeled_send<DebugPause>(&pause);
			break;
		case(GLFW_KEY_F2):
			/* Create a new unit */
			UnitCreateCommand command;
			command.player_id = 0;
			command.x = 50.0f;
			command.y = 50.0f;
			command.z = 0.0f;

			this->channeled_send(&command);
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

void Client::mouseButtonCallback(int button, int action, int mods) {
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

void Client::mouseCursorCallback(double x, double y) {
	if (rightMouseDown) {
		float angle;
		// Perform horizontal (y-axis) rotation
		angle = (float)(lastMousePos.first - x) / 100.0f;
		camera->position = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camera->position, 1.0f));
		camera->up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camera->up, 1.0f));
		//Now rotate vertically based on current orientation
		angle = (float)(y - lastMousePos.second) / 100.0f;
		glm::vec3 axis = glm::cross((camera->position - camera->target), camera->up);
		camera->position = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(camera->position, 1.0f));
		camera->up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(camera->up, 1.0f));
		lastMousePos = std::make_pair(x, y);
	}
}

void Client::mouseWheelCallback(double x, double y) {
	camera->position = glm::vec3(glm::translate(glm::mat4(1.0f), camera->position * (float)y * -0.05f) * glm::vec4(camera->position, 1.0f));
}


void Client::playerUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlayerUpdate> update) {
	update->apply(playerMap[update->id]);

}

void Client::unitUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<UnitUpdate> update) {
	Lib::LOG_DEBUG("Unit update received");
	/*
	update->apply(unitMap[update->id]);
	octree.insert(unitMap[update->id]);
	*/
}

void Client::cityUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<CityUpdate> update) {
	update->apply(cityMap[update->id]);
	octree.insert(cityMap[update->id]);
}

void Client::planetUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlanetUpdate> update) {
	Planet* pl = planetMap[update->id].first;
	update->apply(pl);
	octree.insert(planetMap[update->id].first);
}

void Client::slotUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<SlotUpdate> update) {
	update->apply(slotMap[update->id]);
}

void Client::handle_client_disconnect() {
	/* YEAH? The server wants to disconnect us? WELL LET'S DISCONNECT THEM! */
	Lib::LOG_ERR("The client has been disconnected from the server...");
	this->disconnect();
	Lib::LOG_ERR("Client disconnected.");
}

void Client::handle_client_error() {
	/* If there is some sort of error with the server, just disconnect outright */
	Lib::LOG_ERR("The client could not contact the server..");

	// TODO: Add retry logic/connection fix logic.
	this->disconnect();
	Lib::LOG_ERR("Client disconnected.");
}

void Client::handle_poll_timeout() {}

