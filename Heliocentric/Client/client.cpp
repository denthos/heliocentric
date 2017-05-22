#include "client.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <soil.h>
#include <glad\glad.h>

#include "free_camera.h"
#include "orbital_camera.h"
#include "quad.h"
#include "particle_system.h"
#include "thruster_emitter.h"
#include "laser_emitter.h"
#include "drawable_planet.h"
#include "drawable_unit.h"
#include "skybox_mesh.h"
#include "game_channels.h"
#include "sphere_mesh.h"
#include "transformation.h"
#include "orbit.h"
#include "model.h"
#include "logging.h"
#include "player_client_to_server_xfer.h"
#include "debug_pause.h"
#include "player_command.h"
#include "settle_city_command.h"
#include "unit_command.h"
#include "trade_command.h"
#include "trade_deal.h"
#include "instant_laser_attack.h"

#define VERT_SHADER "Shaders/shader.vert"
#define FRAG_SHADER "Shaders/shader.frag"
#define CUBEMAP_VERT_SHADER "Shaders/cubemap.vert"
#define CUBEMAP_FRAG_SHADER "Shaders/cubemap.frag"
#define TEXTURE_VERT_SHADER "Shaders/simple_texture.vert"
#define TEXTURE_FRAG_SHADER "Shaders/simple_texture.frag"
#define DIFFUSE_FRAG_SHADER "Shaders/diffuse_shader.frag"

#define ROCKET_MODEL "Models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"

//skybox texture files
#define SKYBOX_FRONT "Textures/Skybox/Front_MauveSpaceBox.png"
#define SKYBOX_BACK "Textures/Skybox/Back_MauveSpaceBox.png"
#define SKYBOX_TOP "Textures/Skybox/Up_MauveSpaceBox.png"
#define SKYBOX_BOTTOM "Textures/Skybox/Down_MauveSpaceBox.png"
#define SKYBOX_LEFT "Textures/Skybox/Left_MauveSpaceBox.png"
#define SKYBOX_RIGHT "Textures/Skybox/Right_MauveSpaceBox.png"

#define COLOR_BUFFERS 2
#define REGULAR_BUFFER 0
#define BRIGHTNESS_BUFFER 1

#define CAMERA_SWITCH_KEY "CameraSwitch"

std::unordered_map<GLFWwindow *, Client *> Client::glfwEntry;

Quad * quad; //texture sampler
ParticleSystem* laser_particles;
ParticleSystem* explosion_particles;


SkyboxMesh* skybox;
Shader* shader; //TODO reimplement so it doesn't need to be a pointer on heap?
Shader* textureShader;
Shader* cubemapShader;
Shader * quadShader;
Shader * bloomShader;
Shader * blurShader;
Shader* unitShader;
Shader* unitDeathShader;
Shader* particleShader;

GLuint RBO;
GLuint FBO; //frame buffer for offscreen rendering
			// frame buffers for two pass gaussian blur
GLuint gaussianFBO[COLOR_BUFFERS];
GLuint gaussian_color_buff[COLOR_BUFFERS];

Model * spaceship;
Model * rocket;
//don't forget to clean up afterwards

//multiple render targets to specify more than one frag shader output
GLuint color_buff[COLOR_BUFFERS]; //2 color buffers to attach to frame buffer: 1 for regular scene, one for bright objects

Client::Client() : SunNet::ChanneledClient<SunNet::TCPSocketConnection>(Lib::INIParser::getInstance().get<int>("PollTimeout")) {
	Lib::INIParser & config = Lib::INIParser::getInstance();
	int width = config.get<int>("ScreenWidth");
	int height = config.get<int>("ScreenHeight");

	windowTitle = config.get<std::string>("WindowTitle");
	createWindow(width, height);

	gui = new GUI(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERR("Failed to initialize OpenGL context");
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glEnable(GL_TEXTURE_2D);
  
	////////////////////////////////////////////////////
	//set up buffers for rendering

	//generate and bind frame buffer for scene rendering
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//store rendering operations as texture image to be used in shaders
	//set up a 2D texture attachment for each color buffer
	glGenTextures(COLOR_BUFFERS, color_buff);
	for (GLuint i = 0; i < COLOR_BUFFERS; i++) {

		glBindTexture(GL_TEXTURE_2D, color_buff[i]); //select which texture we're working with
		//allocate memory, but don't fill it, 
		//internal format allows floating point values outside 0.0-1.0
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL); //generate texture the same size as our window

		//choose wrapping options: clamp texture at the edges
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//select type of minification and magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		

		// attach texture to framebuffer by specifying color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buff[i], 0);
	}

	//tell opengl we're rendering to multiple colorbuffers
	GLuint color_attachments[COLOR_BUFFERS] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(COLOR_BUFFERS, color_attachments);

	//render buffer object attachment for frame buffer
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	//make sure opengl is able to do depth testing
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//make sure rendering operations have visual impact on main window by making
	//default frame buffer active again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//set up gaussian blur buffers
	glGenFramebuffers(COLOR_BUFFERS, gaussianFBO);
	glGenTextures(COLOR_BUFFERS, gaussian_color_buff);


	for (GLuint i = 0; i < COLOR_BUFFERS; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);

		//create textures to write rendering to
		glBindTexture(GL_TEXTURE_2D, gaussian_color_buff[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

		//set minification and magnification filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//set wrap functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		//attach texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gaussian_color_buff[i], 0);
		
	}

	///////////////////////////////////////////////////////////////
	quad = new Quad(); 
	

	cameras.push_back(new FreeCamera(keyboard_handler, mouse_handler));
	cameras.push_back(new OrbitalCamera(keyboard_handler, mouse_handler, selection));
	cameras[selectedCamera]->setActive(true);

	init = true;

	resizeCallback(width, height);

	octree.enableViewFrustumCulling(&cameras[selectedCamera]->viewFrustum);

	shader = new Shader(VERT_SHADER, FRAG_SHADER);
	textureShader = new Shader(TEXTURE_VERT_SHADER, TEXTURE_FRAG_SHADER);
	cubemapShader = new Shader(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	unitShader = new Shader("Shaders/geoshader.vert", DIFFUSE_FRAG_SHADER, "Shaders/explode.geom");
	unitDeathShader = new Shader("Shaders/geoshader.vert", DIFFUSE_FRAG_SHADER, "Shaders/explode.geom");
	particleShader = new Shader("Shaders/particle.vert", "Shaders/particle.frag", "Shaders/particle.geom");
	quadShader = new Shader("Shaders/quad.vert", "Shaders/hdr_bloom.frag");
	blurShader = new Shader("Shaders/quad.vert", "Shaders/blur.frag");
	bloomShader = new Shader(TEXTURE_VERT_SHADER, "Shaders/bloom_first_pass.frag");

	

	laser_particles = new ParticleSystem( 0.5f, 1, new LaserEmitter(), particleShader);
	explosion_particles = new ParticleSystem(0.3f, 20, new ParticleEmitter(), particleShader);

	skybox = new SkyboxMesh(SKYBOX_RIGHT, SKYBOX_LEFT, SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_BACK, SKYBOX_FRONT, new SkyboxMeshGeometry());

	for (auto& planet : this->universe.get_planets()) {
		planets[planet->getID()] = std::make_unique<DrawablePlanet>(*planet.get());
		for (auto& slot : planets[planet->getID()]->get_slots_const()) {
			slots.insert(std::make_pair(slot.first, static_cast<DrawableSlot*>(slot.second)));
		}
	}

	// LOAD MODEL, IMPORTANT
	spaceship = Model::getInstance(ROCKET_MODEL);
	rocket = Model::getInstance(ROCKET_MODEL);

	/*UID id1 = 101;
	UID id2 = 102;
	glm::vec3 location(glm::vec3(500.0f, 0.0f, 700.0f));
	glm::vec3 des(glm::vec3(700.0f, 0.0f, 500.0f));
	units[id1] = std::make_unique<DrawableUnit>(
		Unit(location),
		spaceship, unitShader, laser_particles, explosion_particles
		);
	units[id2] = std::make_unique<DrawableUnit>(
		Unit(des),
		spaceship, unitShader, laser_particles, explosion_particles
		);
	units[id1]->set_destination(des);
	units[id1]->do_orient();
	units[id1]->update();*/
	// Set up SunNet client and channel callbacks
	initializeChannels();

	this->subscribe<PlayerUpdate>(std::bind(&Client::playerUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitCreationUpdate>(std::bind(&Client::unitCreationUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitUpdate>(std::bind(&Client::unitUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityUpdate>(std::bind(&Client::cityUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlanetUpdate>(std::bind(&Client::planetUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerIDConfirmation>(std::bind(&Client::playerIdConfirmationHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<TradeData>(std::bind(&Client::tradeDataHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityCreationUpdate>(std::bind(&Client::cityCreationUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));

	int cameraSwitchKey = config.get<std::string>(CAMERA_SWITCH_KEY)[0];
	if (cameraSwitchKey >= 97 && cameraSwitchKey <= 122) cameraSwitchKey -= 32;

	this->keyboard_handler.registerKeyPressHandler(cameraSwitchKey, std::bind(&Client::handleCameraSwitch, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_ESCAPE, std::bind(&Client::handleEscapeKey, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F1, std::bind(&Client::handleF1Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F2, std::bind(&Client::handleF2Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F3, std::bind(&Client::handleF3Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F4, std::bind(&Client::handleF4Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F5, std::bind(&Client::handleF5Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F6, std::bind(&Client::handleF6Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F10, std::bind(&Client::handleF10Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F11, std::bind(&Client::handleF11Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F12, std::bind(&Client::handleF12Key, this, std::placeholders::_1));

	this->mouse_handler.registerMouseClickHandler(MouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_MOD_NONE), 
		std::bind(&Client::mouseClickHandler, this, std::placeholders::_1, std::placeholders::_2));


	std::string address = Lib::INIParser::getInstance().get<std::string>("ServerHost");
	std::string port = Lib::INIParser::getInstance().get<std::string>("ServerPort");
	try {
		this->connect(address, port);
	}
	catch (const SunNet::ConnectException&) {
		LOG_ERR("Could not connect to host at address ", address, " and port ", port);
	}
}

Client::~Client() {
	delete shader;
	delete textureShader;

	players.clear();
	planets.clear();
	units.clear();
	cities.clear();
	slots.clear();

	glfwEntry.erase(window);
}

bool Client::isRunning() {
	return !glfwWindowShouldClose(window);
}

void Client::createWindow(int width, int height) {
	if (!glfwInit()) {
		LOG_ERR("Could not initialize GLFW");
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
		LOG_ERR("Could not create GLFW window");
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);

	glfwEntry[window] = this;

	glfwSetErrorCallback(
		[](int error, const char * description) {
			Client::errorCallback(error, description);
		}
	);

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow * window, int width, int height) {
			glfwEntry[window]->resizeCallback(width, height);
		}
	);

	glfwSetKeyCallback(window,
		[](GLFWwindow * window, int key, int scancode, int action, int mods) {
			glfwEntry[window]->keyCallback(key, scancode, action, mods);
		}
	);

	glfwSetCharCallback(window,
		[](GLFWwindow * window, unsigned int codepoint) {
			glfwEntry[window]->gui->charCallbackEvent(codepoint);
		}
	);

	glfwSetDropCallback(window,
		[](GLFWwindow * window, int count, const char **filenames) {
			glfwEntry[window]->gui->dropCallbackEvent(count, filenames);
		}
	);

	glfwSetMouseButtonCallback(window,
		[](GLFWwindow * window, int button, int action, int mods) {
			glfwEntry[window]->mouseButtonCallback(button, action, mods);
		}
	);

	glfwSetCursorPosCallback(window,
		[](GLFWwindow * window, double x, double y) {
			glfwEntry[window]->mouseCursorCallback(x, y);
		}
	);
	glfwSetScrollCallback(window,
		[](GLFWwindow * window, double x, double y) {
			glfwEntry[window]->scrollWheelCallback(x, y);
		}
	);
}

void Client::display() {
  //first pass: render the scene as usual with the bloom framebuffer as the active frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameras[selectedCamera]->update();
	cameras[selectedCamera]->calculateViewMatrix();
	cameras[selectedCamera]->calculateViewFrustum();

	octree.clear();

	for (auto it = planets.begin(); it != planets.end(); ++it) {
		octree.insert((*it).second.get());
	}
	for (auto it = units.begin(); it != units.end(); ++it) {
		octree.insert((*it).second.get());
	}
	for (auto it = cities.begin(); it != cities.end(); ++it) {
		octree.insert((*it).second.get());
	}
	for (auto it = slots.begin(); it != slots.end(); ++it) {
		octree.insert((*it).second);
	}
	octree.update();

	skybox->draw(*cubemapShader, *cameras[selectedCamera], glm::scale(glm::mat4(1.0f), glm::vec3(4000.0f)));
	octree.draw(*textureShader, *cameras[selectedCamera]);


	// Deal with dead objects.
	auto& dead_it = dead_objects.begin();
	while (dead_it != dead_objects.end()) {
		dead_it->second->update();
		if (dead_it->second->do_animation(*textureShader, *cameras[selectedCamera]) == true) {
			UID id = dead_it->first;
			LOG_DEBUG("Deleting unit with id " + std::to_string(id));
			dead_it++;
			auto& update_queue = Lib::key_acquire(this->update_queue);
			update_queue.get().push([id, this]() {
				dead_objects.erase(id);;
			});
		}
		else {
			dead_it++;
		}
	}

	
	// blur the things that glow
	int blurs = 5; //TODO init to number of blur iterations
	bool blurX = true;

	blurShader->bind();

	//fill one of the gaussian frame buffers with the blurred bright extraction
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[blurX]);
	glBindTexture(GL_TEXTURE_2D, color_buff[BRIGHTNESS_BUFFER]);
	glUniform1i(glGetUniformLocation(blurShader->getPid(), "blurX"), blurX);

	quad->draw();
	blurX = !blurX;

	//blur the image
	for (int i = 0; i < blurs; i++) { 
		
		//bind appropriate frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[blurX]);
		//bind appropriate texture to blur (the one opposite to the current frame buffer you're writing to)
		glBindTexture(GL_TEXTURE_2D, gaussian_color_buff[!blurX]);

		blurX = !blurX; //switch between blur directions

		//draw texture
		quad->draw();
	}
	
	blurShader->unbind();

	//return to the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw the quad 
	quadShader->bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_buff[REGULAR_BUFFER]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gaussian_color_buff[!blurX]);
	
	glUniform1i(glGetUniformLocation(quadShader->getPid(), "sceneTexture"), 0);
	glUniform1i(glGetUniformLocation(quadShader->getPid(), "blurTexture"), 1);
	glUniform1f(glGetUniformLocation(quadShader->getPid(), "gammaFactor"), 1.72f);
	glUniform1f(glGetUniformLocation(quadShader->getPid(), "exposure"), 2.0f);
	quad->draw();

	quadShader->unbind();

	// draw the UI
	gui->drawWidgets();

	glfwSwapBuffers(window);

	glfwPollEvents();
}

void Client::update() {

	cameras[selectedCamera]->update();

	//particles->Update(*camera);
	
	this->keyboard_handler.callKeyboardHandlers();

	auto& update_queue = Lib::key_acquire(this->update_queue);
	while (!update_queue.get().empty()) {
		update_queue.get().back()();
		update_queue.get().pop();
	}
}

void Client::errorCallback(int error, const char * description) {
	LOG_ERR("OpenGL Error occurred: ", description);
}

void Client::resizeCallback(int width, int height) {
	if (!init) return;
	// resize the camera
	for (auto camera : cameras) {
		camera->width = width;
		camera->height = height;
		camera->aspectRatio = (float)width / (float)height;
		camera->calculatePerspectiveMatrix();
		camera->calculateInfinitePerspectiveMatrix();
	}

	// resize our buffers
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	for (GLuint i = 0; i < COLOR_BUFFERS; i++) {
		glBindTexture(GL_TEXTURE_2D, color_buff[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	for (GLuint i = 0; i < COLOR_BUFFERS; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		glBindTexture(GL_TEXTURE_2D, gaussian_color_buff[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	}

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glViewport(0, 0, width, height);

	if (gui) {
		gui->resizeCallbackEvent(width, height);
	}
}

void Client::keyCallback(int key, int scancode, int action, int mods) {
	if (!gui->keyCallbackEvent(key, scancode, action, mods))
		keyboard_handler.keyCallback(key, scancode, action, mods);
}

void Client::mouseButtonCallback(int button, int action, int mods) {
	if (!gui->mouseButtonCallbackEvent(button, action, mods)) {
		mouse_handler.mouseButtonCallback(button, action, mods);
	}
}

void Client::mouseCursorCallback(double x, double y) {
	if (!gui->cursorPosCallbackEvent(x, y)) {
		mouse_handler.mouseCursorCallback(x, y);
	}
}

void Client::scrollWheelCallback(double x, double y) {
	if (!gui->scrollCallbackEvent(x, y)) {
		mouse_handler.mouseWheelCallback(x, y);
	}
}

void Client::mouseClickHandler(MouseButton mouseButton, ScreenPosition position) {
	selection.clear();
	cameras[selectedCamera]->calculateViewMatrix();
	GameObject * selected = dynamic_cast<GameObject *>(octree.intersect(cameras[selectedCamera]->projectRay(position)));
	if (selected) {
		selection.push_back(selected);
		gui->updateSelection(selected);
		LOG_INFO("Selected game object with UID <", selected->getID(), ">");
	}
}

void Client::handleCameraSwitch(int key) {
	cameras[selectedCamera]->setActive(false);
	selectedCamera++;
	if (selectedCamera >= cameras.size()) selectedCamera = 0;
	octree.enableViewFrustumCulling(&cameras[selectedCamera]->viewFrustum);
	cameras[selectedCamera]->setActive(true);
}

void Client::handleEscapeKey(int key) {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Client::handleF1Key(int key) {
	DebugPause pause;
	this->channeled_send<DebugPause>(&pause);
}

void Client::handleF2Key(int key) {
	/* Find the first player that is not this client and send him a trade deal ^^ */
	UID recipient;
	bool recipient_found = false;
	for (auto it = players.begin(); it != players.end(); it++) {
		LOG_DEBUG("Iterating through player with id: ", it->first);
		if (it->second != this->player) {
			recipient = it->first;
			recipient_found = true;
			LOG_DEBUG("Recipient found with id: ", recipient);
			break;
		}
	}

	if (!recipient_found) {
		LOG_ERR("Cannot send trade deal because no other player is in game.");
		return;
	}

	TradeData deal(this->player->getID(), recipient, Resources::GOLD, 10);
	this->channeled_send(&deal);
}

void Client::handleF3Key(int key) {

	PlayerCommand command(rand() % 500, rand() % 500, rand() % 500);

	this->channeled_send(&command);
}

void Client::handleF4Key(int key) {

	auto unit_it = units.begin();
	if (units.size() == 0) {
		return;
	}
	std::advance(unit_it, rand() % units.size());
	UnitCommand command(unit_it->first, (float)(rand() % 1000), (float)(rand() % 1000), (float)(rand() % 1000));
	this->channeled_send(&command);
}


void Client::handleF5Key(int key) {
	/* we are going to create a city on the selected slot... */
	if (this->selection.size() <= 0 || this->selection.size() > 1) {
		LOG_DEBUG("Cannot settle when nothing is selected...");
		return;
	}

	Slot* slot = dynamic_cast<Slot*>(this->selection[0]);
	if (!slot) {
		LOG_DEBUG("Cannot settle when thing selected is not a slot");
		return;
	}

	SettleCityCommand settle_command(slot->getID());
	this->channeled_send(&settle_command);
}

void Client::handleF6Key(int key) {
	UID attacker, enemy;
	bool found_attacker = false;
	bool found_enemy = false;
	for (auto iter = units.begin(); iter != units.end(); ++iter) {
		if (iter->second->get_player()->getID() == player->getID()) {
			if (!found_attacker) {
				attacker = iter->second->getID();
				found_attacker = true;
				LOG_DEBUG("Selected your unit with id " + std::to_string(attacker));
			}
		}
		else {
			if (!found_enemy) {
				enemy = iter->second->getID();
				found_enemy = true;
				LOG_DEBUG("Selected enemy unit with id " + std::to_string(enemy));
			}
		}
		if (found_attacker && found_enemy) {
			LOG_DEBUG("Selected your unit with id " + std::to_string(attacker) + " to attack enemy with id " + std::to_string(enemy));
			UnitCommand command(attacker, enemy);
			this->channeled_send(&command);
			return;
		}
	}

	LOG_ERR("Failed to find a unit to attack.");
}

void Client::handleF10Key(int key) {
	// Counter offer a trade deal giving for free what the original seller wanted to give -- yeah I know these guys are crazy.
	std::shared_ptr<TradeDeal> trade_deal;
	try {
		/* Get the first trade deal from player's pending list */
		trade_deal = player->get_trade_deal();
	}
	catch (Identifiable::BadUIDException e) {
		LOG_ERR("No pending trade deal found");
		return;
	}

	if (this->player->getID() != trade_deal->get_recipient()) {
		LOG_ERR("Player has a deal that wasn't sent to this player -- weird!");
		return;
	}

	/* Start a counter-offer based on the found trade deal */
	/* TODO: Maybe there's a better way to make "modifying" a deal easier */
	TradeData deal(trade_deal->get_recipient(), trade_deal->get_sender(), trade_deal->get_sell_type(), trade_deal->get_sell_amount() * 2);
	this->channeled_send(&deal);
}

void Client::handleF11Key(int key) {
	// Accept the first trade deal in player's pending map
	UID deal_id = player->trade_deal_accept();
	if (deal_id == 0) {
		LOG_ERR("No pending trade deal found");
		return;
	}

	TradeCommand command(deal_id, true);
	this->channeled_send(&command);
}

void Client::handleF12Key(int key) {
	// Decline the first trade deal in player's pending map
	UID deal_id = player->trade_deal_decline();
	if (deal_id == 0) {
		LOG_ERR("No pending trade deal found");
		return;
	}

	TradeCommand command(deal_id, false);
	this->channeled_send(&command);
}

void Client::playerUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlayerUpdate> update) {
	LOG_DEBUG("Received update for player with id: ", update->id);
	auto& player_it = players.find(update->id);
	if (player_it == players.end()) {
		players[update->id] = std::make_shared<Player>(update->player_name, update->id);
	}

	if (this->player->getID() == update->id) {
		LOG_DEBUG("Player gold amount before trading: ", players[update->id]->get_resource_amount(Resources::GOLD));
	}
	update->apply(players[update->id].get());

	if (this->player->getID() == update->id) {
		LOG_DEBUG("Player gold amount after trading: ", players[update->id]->get_resource_amount(Resources::GOLD));
	}
}

void Client::unitCreationUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<UnitCreationUpdate> update) {
	LOG_DEBUG("Unit creation update received");
	Lib::assertTrue(players.find(update->player_id) != players.end(), "Invalid player ID");
	units[update->id] = std::make_unique<DrawableUnit>(
		Unit(update->id, glm::vec3(update->x, update->y, update->z), players[update->player_id].get(), new InstantLaserAttack(), update->def, update->range, update->health),
		spaceship, unitShader, laser_particles, explosion_particles
	);
}

void Client::cityCreationUpdateHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<CityCreationUpdate> update) {
	LOG_DEBUG("City creation update received");
	auto& player_iter = players.find(update->player_id);
	Lib::assertTrue(player_iter != players.end(), "Invalid player ID");

	auto& slot_iter = slots.find(update->slot_id);
	Lib::assertTrue(slot_iter != slots.end(), "Invalid slot id");

	auto& update_queue = Lib::key_acquire(this->update_queue);
	std::function<void()> createCityFunc = [slot_iter, update, player_iter]() {
		slot_iter->second->attachCity(new DrawableCity(City(update->city_id, player_iter->second.get(), new InstantLaserAttack(), 0, 0, 0, 0, slot_iter->second)));
	};

	update_queue.get().push(createCityFunc);
}

void Client::playerIdConfirmationHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerIDConfirmation> update) {
	LOG_DEBUG("Received Player ID confirmation -- I am player with id ", update->id);
	std::string player_name = Lib::INIParser::getInstance().get<std::string>("PlayerName");
	this->player = std::make_shared<Player>(player_name, update->id);
	players[update->id] = this->player;

	PlayerClientToServerTransfer info_transfer(player_name);

	sender->channeled_send<PlayerClientToServerTransfer>(&info_transfer);
}

void Client::unitUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<UnitUpdate> update) {
	//LOG_DEBUG("Unit update received");
	update->apply(units[update->id].get());
	units[update->id]->update();

	/* 
	Handle unit death. We don't want to edit the units map in another thread,
	so let's pop it in the main thread's queue
	*/
	LOG_DEBUG("Unit with ID " + std::to_string(update->id) + " health is " + std::to_string(units[update->id]->get_health()));
	if (units[update->id].get()->is_dead()) {
		units[update->id]->is_exploding = true;
		units[update->id]->bind_shader(unitDeathShader);
		auto& update_queue = Lib::key_acquire(this->update_queue);
		update_queue.get().push([update, this]() {
			dead_objects.insert(std::make_pair(update->id, std::move(units[update->id])));
			units.erase(update->id);
		});
	}
}

void Client::cityUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<CityUpdate> update) {
	update->apply(cities[update->id].get());
}

void Client::planetUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlanetUpdate> update) {
	update->apply(planets[update->id].get());
	planets[update->id]->update();
}

void Client::tradeDataHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeData> deal) {
	LOG_DEBUG("Received a trade deal");
	Lib::assertTrue(players.find(deal->sender) != players.end() && players.find(deal->recipient) != players.end(), "Invalid player ID");

	if (this->player->getID() == deal->sender) {
		LOG_DEBUG("I am the sender of this trade deal");
		// For now, we don't have to do anything if this player sent this trade deal.
	}
	else {
		LOG_DEBUG("I am the receiver of this trade deal");
		/* Create a TradeDeal from TradeData and store it into player's pending trade deals */
		player->receive_trade_deal(std::make_shared<TradeDeal>(deal, deal->trade_deal_id));
	}
}

void Client::handle_client_disconnect() {
	/* YEAH? The server wants to disconnect us? WELL LET'S DISCONNECT THEM! */
	LOG_ERR("The client has been disconnected from the server...");
	this->disconnect();
	LOG_ERR("Client disconnected.");
}

void Client::handle_client_error() {
	/* If there is some sort of error with the server, just disconnect outright */
	LOG_ERR("The client could not contact the server..");

	// TODO: Add retry logic/connection fix logic.
	this->disconnect();
	LOG_ERR("Client disconnected.");
}

void Client::handle_poll_timeout() {}

