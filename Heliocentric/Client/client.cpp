#include "client.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <stdio.h>
#include <soil.h>



#include "drawable_planet.h"
#include "skybox_mesh.h"
#include "glfw_callback_handler.h"
#include "game_channels.h"
#include "sphere_mesh.h"
#include "transformation.h"
#include "orbit.h"
#include "model.h"
#include "logging.h"

#include "player_client_to_server_xfer.h"
#include "debug_pause.h"
#include "player_command.h"


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

Client::Client() : SunNet::ChanneledClient<SunNet::TCPSocketConnection>(Lib::INIParser::getInstance().get<int>("PollTimeout")) {
	Lib::INIParser & config = Lib::INIParser::getInstance("config.ini");
	int width = config.get<int>("ScreenWidth");
	int height = config.get<int>("ScreenHeight");
	windowTitle = config.get<std::string>("WindowTitle");
	createWindow(width, height);

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

	camera = new Camera(glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 1.0f, 10000.0f, width, height);

	resizeCallback(width, height);

	shader = new Shader(VERT_SHADER, FRAG_SHADER);
	textureShader = new Shader(TEXTURE_VERT_SHADER, TEXTURE_FRAG_SHADER);
	cubemapShader = new Shader(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);

	skybox = new SkyboxMesh(SKYBOX_RIGHT, SKYBOX_LEFT, SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_BACK, SKYBOX_FRONT);
	//TODO rewrite this
	//skybox->world_mat =  glm::scale(skybox->world_mat, glm::vec3(4000.0f));

	for (auto& planet : this->universe.get_planets()) {
		//auto earth_model = new PlanetModel(Texture(EARTH_TEXTURE), planet->get_radius(), Orbit(0.0f, 0.0f));
		//this->planetMap[planet->getID()] = std::make_pair(planet.get(), earth_model);
		DrawablePlanet * drawablePlanet = new DrawablePlanet(*planet.get(), Texture(EARTH_TEXTURE));
		gameObjects[planet->getID()] = drawablePlanet;
		planets[planet->getID()] = drawablePlanet;
	}

	// Set up SunNet client and channel callbacks
	initializeChannels();

	this->subscribe<PlayerUpdate>(std::bind(&Client::playerUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitUpdate>(std::bind(&Client::unitUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityUpdate>(std::bind(&Client::cityUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlanetUpdate>(std::bind(&Client::planetUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<SlotUpdate>(std::bind(&Client::slotUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerIDConfirmation>(std::bind(&Client::playerIdConfirmationHandler, this, std::placeholders::_1, std::placeholders::_2));

	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_ESCAPE, std::bind(&Client::handleEscapeKey, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F1, std::bind(&Client::handleF1Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F3, std::bind(&Client::handleF3Key, this, std::placeholders::_1));

	std::string address = Lib::INIParser::getInstance().get<std::string>("ServerHost");
	std::string port = Lib::INIParser::getInstance().get<std::string>("ServerPort");
	try {
		this->connect(address, port);
	}
	catch (const SunNet::ConnectException&) {
		Lib::LOG_ERR("Could not connect to host at address ", address, " and port ", port);
	}
}

Client::~Client() {
	delete shader;
	delete textureShader;

	for (auto & gameObject : gameObjects) {
		delete gameObject.second;
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

	for (auto & gameObject : gameObjects) {
		octree.insert(gameObject.second);
	}

	//octree.viewFrustumCull(ViewFrustum()); // TODO: get view frustum from camera
	octree.draw(*textureShader, *camera);
	octree.clear(); // empty octree each frame

	skybox->draw(*cubemapShader, *camera, glm::scale(glm::mat4(1.0f), glm::vec3(4000.0f)));

	glfwSwapBuffers(window);

	glfwPollEvents();
}

void Client::update() {
	this->keyboard_handler.callKeyboardHandlers();
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
		this->keyboard_handler.setKeyDown(key);
	}
	else if (action == GLFW_RELEASE) {
		this->keyboard_handler.setKeyUp(key);
	}
}

void Client::handleF1Key(int key) {
	DebugPause pause;
	this->channeled_send<DebugPause>(&pause);
}

void Client::handleEscapeKey(int key) {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Client::handleF3Key(int key) {
	PlayerCommand command;
	command.command_type = BaseCommand::CMD_CREATE;
	command.create_location_x = 50.0f;
	command.create_location_y = 50.0f;
	command.create_location_z = 0.0f;

	this->channeled_send(&command);
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
	Lib::LOG_DEBUG("Received update for player with id: ", update->id);
	auto& player_it = players.find(update->id);
	if (player_it == players.end()) {
		Player* new_player = new Player(update->player_name, update->id);
		players[update->id] = new_player;
	}

	update->apply(players[update->id]);

}


void Client::playerIdConfirmationHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerIDConfirmation> update) {
	Lib::LOG_DEBUG("Received Player ID confirmation -- I am player with id ", update->id);
	std::string player_name = Lib::INIParser::getInstance("config.ini").get<std::string>("PlayerName");
	this->player = new Player(player_name, update->id);
	players[update->id] = this->player;

	PlayerClientToServerTransfer info_transfer;
	snprintf(info_transfer.name, PLAYER_NAME_MAX_SIZE, "%s", player_name.c_str());

	sender->channeled_send<PlayerClientToServerTransfer>(&info_transfer);
}

void Client::unitUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<UnitUpdate> update) {
	update->apply(units[update->id]);
	Lib::LOG_DEBUG("Unit update received");
}

void Client::cityUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<CityUpdate> update) {
	update->apply(cities[update->id]);
}

void Client::planetUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlanetUpdate> update) {
	update->apply(planets[update->id]);
	gameObjects[update->id]->update();
}

void Client::slotUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<SlotUpdate> update) {
	update->apply(slots[update->id]);
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

