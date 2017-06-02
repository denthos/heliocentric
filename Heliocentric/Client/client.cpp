#include "client.h"

#include <glad\glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <soil.h>
#include <thread>

#include "free_camera.h"
#include "model_preloader.h"
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
#include "selectable.h"
#include "unit_spawner_update.h"

#define ALLOWED_ACTIONS_PER_TICK 200

#define VERT_SHADER "Shaders/shader.vert"
#define FRAG_SHADER "Shaders/shader.frag"
#define CUBEMAP_VERT_SHADER "Shaders/cubemap.vert"
#define CUBEMAP_FRAG_SHADER "Shaders/cubemap.frag"
#define TEXTURE_VERT_SHADER "Shaders/simple_texture.vert"
#define TEXTURE_FRAG_SHADER "Shaders/simple_texture.frag"
#define DIFFUSE_FRAG_SHADER "Shaders/diffuse_shader.frag"

#define ROCKET_MODEL "Federation Interceptor HN48 flying.obj"

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
ParticleSystem* particles;


SkyboxMesh* skybox;
Shader* shader; //TODO reimplement so it doesn't need to be a pointer on heap?
Shader* textureShader;
Shader* cubemapShader;
Shader * quadShader;
Shader * bloomShader;
Shader * blurShader;
Shader* diffuseShader;
Shader* colorShader;
Shader* particleShader;

GLuint RBO;
GLuint FBO; //frame buffer for offscreen rendering
			// frame buffers for two pass gaussian blur
GLuint gaussianFBO[COLOR_BUFFERS];
GLuint gaussian_color_buff[COLOR_BUFFERS];


//don't forget to clean up afterwards

//multiple render targets to specify more than one frag shader output
GLuint color_buff[COLOR_BUFFERS]; //2 color buffers to attach to frame buffer: 1 for regular scene, one for bright objects

Client::Client() : SunNet::ChanneledClient<SunNet::TCPSocketConnection>(Lib::INIParser::getInstance().get<int>("PollTimeout")) {
	Lib::INIParser & config = Lib::INIParser::getInstance();
	int width = config.get<int>("ScreenWidth");
	int height = config.get<int>("ScreenHeight");

	windowTitle = config.get<std::string>("WindowTitle");
	createWindow(width, height);

	gui = new GUI(window, width, height);

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
	focused = true;

	resizeCallback(width, height);

	octree = new Octree();
	octree->enableViewFrustumCulling(&cameras[selectedCamera]->viewFrustum);

	shader = new Shader(VERT_SHADER, FRAG_SHADER);
	textureShader = new Shader(TEXTURE_VERT_SHADER, TEXTURE_FRAG_SHADER);
	cubemapShader = new Shader(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	diffuseShader = new Shader("Shaders/shader.vert", DIFFUSE_FRAG_SHADER);
	colorShader = new Shader("Shaders/shader.vert", "Shaders/color_shader.frag");
	//diffuseShader = new Shader("Shaders/geoshader.vert", DIFFUSE_FRAG_SHADER, "Shaders/explode.geom");
	particleShader = new Shader("Shaders/particle.vert", "Shaders/particle.frag", "Shaders/particle.geom");
	quadShader = new Shader("Shaders/quad.vert", "Shaders/hdr_bloom.frag");
	blurShader = new Shader("Shaders/quad.vert", "Shaders/blur.frag");
	bloomShader = new Shader(TEXTURE_VERT_SHADER, "Shaders/bloom_first_pass.frag");

	particles = new ParticleSystem(0.0f, 20, new ParticleEmitter());

	skybox = new SkyboxMesh(SKYBOX_RIGHT, SKYBOX_LEFT, SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_BACK, SKYBOX_FRONT, new SkyboxMeshGeometry());

	// LOAD MODELS, IMPORTANT
	ModelPreloader::preload();

	for (auto& planet : this->universe.get_planets()) {
		planets[planet->getID()] = std::make_unique<DrawablePlanet>(*planet.get(), textureShader, diffuseShader);
		for (auto& slot : planets[planet->getID()]->get_slots_const()) {
			slots.insert(std::make_pair(slot.first, static_cast<DrawableSlot*>(slot.second)));
		}
	}

	// Set up SunNet client and channel callbacks
	initializeChannels();

	this->subscribe<PlayerUpdate>(std::bind(&Client::playerUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<NewPlayerInfoUpdate>(std::bind(&Client::newPlayerInfoUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitCreationUpdate>(std::bind(&Client::unitCreationUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitUpdate>(std::bind(&Client::unitUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityUpdate>(std::bind(&Client::cityUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlanetUpdate>(std::bind(&Client::planetUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerIDConfirmation>(std::bind(&Client::playerIdConfirmationHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerScoreUpdate>(std::bind(&Client::playerScoreUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<TradeData>(std::bind(&Client::tradeDataHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<CityCreationUpdate>(std::bind(&Client::cityCreationUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<SlotUpdate>(std::bind(&Client::slotUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<GameOverUpdate>(std::bind(&Client::gameOverUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitSpawnerUpdate>(std::bind(&Client::unitSpawnerUpdateHandler, this, std::placeholders::_1, std::placeholders::_2));

	int cameraSwitchKey = config.get<std::string>(CAMERA_SWITCH_KEY)[0];
	if (cameraSwitchKey >= 97 && cameraSwitchKey <= 122) cameraSwitchKey -= 32;

	this->keyboard_handler.registerKeyPressHandler(cameraSwitchKey, std::bind(&Client::handleCameraSwitch, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_ESCAPE, std::bind(&Client::handleEscapeKey, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F1, std::bind(&Client::handleF1Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F2, std::bind(&Client::handleF2Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F4, std::bind(&Client::handleF4Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F6, std::bind(&Client::handleF6Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F10, std::bind(&Client::handleF10Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F11, std::bind(&Client::handleF11Key, this, std::placeholders::_1));
	this->keyboard_handler.registerKeyPressHandler(GLFW_KEY_F12, std::bind(&Client::handleF12Key, this, std::placeholders::_1));

	this->mouse_handler.registerMouseClickHandler(MouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_MOD_NONE), 
		std::bind(&Client::mouseClickHandler, this, std::placeholders::_1, std::placeholders::_2));
	this->mouse_handler.registerMouseClickHandler(MouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_NONE),
		std::bind(&Client::mouseRightClickHandler, this, std::placeholders::_1, std::placeholders::_2));


	std::string address = Lib::INIParser::getInstance().get<std::string>("ServerHost");
	std::string port = Lib::INIParser::getInstance().get<std::string>("ServerPort");
	try {
		this->connect(address, port);
	}
	catch (const SunNet::ConnectException&) {
		LOG_ERR("Could not connect to host at address ", address, " and port ", port);
	}

	frameTimer = glfwGetTime();
	frameCounter = 0;

	musicPlayer.load_sound("Audio/Holst_The_Planets_Mars.ogg");
	musicPlayer.load_sound("Audio/Holst_The_Planets_Jupiter.ogg");
	musicPlayer.load_sound("Audio/Holst_The_Planets_Venus.ogg");
	musicPlayer.load_sound("Audio/Holst_The_Planets_Uranus.ogg");
	musicPlayer.load_sound("Audio/Holst_The_Planets_Mercury.ogg");
	sound_thread = std::thread([&](MusicPlayer* player) { player->play(); }, &musicPlayer);
}

Client::~Client() {
	delete shader;
	delete textureShader;

	delete octree;

	players.clear();
	planets.clear();
	units.clear();
	cities.clear();
	slots.clear();

	glfwEntry.erase(window);

	/* Stop sound thread */
	musicPlayer.stop();
	sound_thread.join();
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
	glfwSetWindowFocusCallback(window,
		[](GLFWwindow * window, int focused) {
			glfwEntry[window]->focusCallback(focused);
		}
	);
}

void Client::display() {
	if (focused || frameCounter % 3 == 0) {
		//first pass: render the scene as usual with the bloom framebuffer as the active frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cameras[selectedCamera]->update();
		cameras[selectedCamera]->calculateViewMatrix();
		cameras[selectedCamera]->calculateViewFrustum();

		Octree * newOctree = new Octree();
		newOctree->enableViewFrustumCulling(&cameras[selectedCamera]->viewFrustum);

		for (auto it = planets.begin(); it != planets.end(); ++it) {
			newOctree->insert((*it).second.get());
		}
		for (auto it = units.begin(); it != units.end(); ++it) {
			newOctree->insert((*it).second.get());
		}
		for (auto it = cities.begin(); it != cities.end(); ++it) {
			newOctree->insert((*it).second.get());
		}
		for (auto it = slots.begin(); it != slots.end(); ++it) {
			newOctree->insert((*it).second);
		}
		newOctree->update();


		skybox->draw(*cubemapShader, *cameras[selectedCamera], glm::scale(glm::mat4(1.0f), glm::vec3(4000.0f)));

		Octree * delOctree = octree;
		octree = newOctree;
		octree->draw(*cameras[selectedCamera]);
		delete delOctree;

		//rocket.draw(*diffuseShader, *camera, glm::mat4(1.0f));
		//particles->draw(*particleShader, *camera, glm::mat4(1.0f));

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

	}


	double currentTime = glfwGetTime();
	frameCounter++;
	if (currentTime - frameTimer >= 1.0) {
		gui->setFPS(1000.0 / (double)(frameCounter - lastFrame));
		lastFrame = frameCounter;
		//frameTimer = currentTime;
		frameTimer += 1.0;
	}


	glfwPollEvents();
}

void Client::update() {

	int action_counter = 0;
	bool has_more_updates = true;
	while (has_more_updates && action_counter++ < ALLOWED_ACTIONS_PER_TICK) {
		has_more_updates = this->poll();
	}

	if (action_counter >= ALLOWED_ACTIONS_PER_TICK) {
		LOG_WARN("Client performed ", action_counter, "in a single tick. Lots of stuff is being sent...");
	}

	cameras[selectedCamera]->update();
	
	gui->update();

	//particles->Update(*camera);
	
	this->keyboard_handler.callKeyboardHandlers();
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
		gui->setScreenSize(width, height);
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

void Client::focusCallback(int focused) {
	this->focused = focused ? true : false;
}

void Client::setSelection(std::vector<GameObject*> new_selection) {
	/* Deselect everything on the GUI */
	gui->unselectSelection(this, selection);
	
	/* Clear the selection */
	selection.clear();

	/* Apply the new selection */
	selection = new_selection;

	/* Update the GUI */
	gui->selectSelection(this, selection);
}


GameObject* Client::getObjectAtCursorPosition(const ScreenPosition& position) {
	cameras[selectedCamera]->calculateViewMatrix();
	return dynamic_cast<GameObject *>(octree->intersect(cameras[selectedCamera]->projectRay(position)));
}


void Client::mouseClickHandler(MouseButton mouseButton, ScreenPosition position) {
	GameObject* selected_obj = getObjectAtCursorPosition(position);

	if (selected_obj) {
		setSelection({ selected_obj });
	}
}

void Client::mouseRightClickHandler(MouseButton mouseButton, ScreenPosition position) {
	/* Right clicks are only useful if we currently have a selection and the selection is all ours */
	if (this->selection.size() <= 0) {
		return;
	}
	else {
		for (GameObject* single_selection : this->selection) {
			if (!single_selection->has_player() || single_selection->get_player()->getID() != this->player->getID()) {
				return;
			}
		}
	}

	GameObject* selected_obj = getObjectAtCursorPosition(position);
	if (!selected_obj) {
		return;
	}

	/* If the right clicked thing is not unit or it is owned by the current player, move */
	glm::vec3 obj_position = selected_obj->get_position();
	AttackableGameObject* selected_attackable = dynamic_cast<AttackableGameObject*>(selected_obj);

	/* come to think of it this should probably all be done on the server.. */
	if (!selected_obj->has_player() || selected_obj->get_player()->getID() == this->player->getID() || !selected_attackable) {
		LOG_DEBUG("Moving selection to position <", obj_position.x, ",", obj_position.y, ",", obj_position.z, ">");

		for (GameObject* single_selection : selection) {
			UnitCommand move_command(single_selection->getID(), obj_position.x, obj_position.y, obj_position.z);
			channeled_send(&move_command);
		}
	}
	/* The right clicked thing is a unit and not owned by the current player */
	else {
		LOG_DEBUG("Selection attacking unit with ID ", selected_obj->getID());

		for (GameObject* single_selection : selection) {
			UnitCommand attack_command(single_selection->getID(), selected_obj->getID());
			channeled_send(&attack_command);
		}
	}
}

void Client::handleCameraSwitch(int key) {
	cameras[selectedCamera]->setActive(false);
	selectedCamera++;
	if (selectedCamera >= cameras.size()) selectedCamera = 0;
	octree->enableViewFrustumCulling(&cameras[selectedCamera]->viewFrustum);
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

	TradeData deal(this->player->getID(), recipient, Resources::GOLD, 10, Resources::GOLD, 10);
	this->channeled_send(&deal);
}


void Client::createUnitFromCity(DrawableCity* city, UnitType* unit_type) {
	/* First, let's check to see if the client-side player has enough resources */
	if (!unit_type->hasBuildRequirements(this->player->getResources())) {
		/* Cannot create the unit due to lack of sufficient resources */
		return;
	}

	glm::vec3 pos = city->get_position() + glm::vec3(city->get_slot()->get_spherical_position().getRotationMatrix() * glm::vec4(0.0f, city->get_slot()->getPlanet()->get_radius() * 1.15f, 0.0f, 0.0f));
	PlayerCommand command(pos.x, pos.y, pos.z, unit_type->getIdentifier(), city->getID());

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

void Client::gameOverUpdateHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<GameOverUpdate> update) {
	gui->showGameOverWindow(update->winnerID == this->player->getID());
}


void Client::createCityForSlot(DrawableSlot* slot, std::string city_name) {
	SettleCityCommand settle_command(slot->getID(), city_name);
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
	TradeData deal(trade_deal->get_recipient(), trade_deal->get_sender(), trade_deal->get_sell_type(),
		trade_deal->get_sell_amount(), trade_deal->get_buy_type(), trade_deal->get_buy_amount() * 2);
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


void Client::newPlayerInfoUpdateHandler(SunNet::ChanneledSocketConnection_p conn, std::shared_ptr<NewPlayerInfoUpdate> update) {
	auto& player_it = players.find(update->player_id);

	std::shared_ptr<Player> player_info;
	if (player_it == players.end()) {
		LOG_DEBUG("Received information about new player (ID: ", update->player_id, " NAME: ", update->name, ")");
		player_info = std::make_shared<Player>(update->name, update->player_id, update->color);
		players[update->player_id] = player_info;
	}
	else {
		player_info = player_it->second;
		update->apply(player_info.get());
	}

	gui->updatePlayerLeaderboardValue(player_info.get());
}

void Client::playerUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlayerUpdate> update) {
	LOG_DEBUG("Received update for player with id: ", update->id);
	auto& player_it = players.find(update->id);
	if (player_it == players.end()) {
		return;
	}

	update->apply(players[update->id].get());
}

void Client::playerScoreUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlayerScoreUpdate> update) {
	auto& player_it = players.find(update->id);
	if (player_it == players.end()) {
		return;
	}

	update->apply(players[update->id].get());
	gui->updatePlayerLeaderboardValue(player_it->second.get());
}

void Client::unitCreationUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<UnitCreationUpdate> update) {
	LOG_DEBUG("Unit creation update received");
	auto& player_it = players.find(update->player_id);
	Lib::assertNotEqual(player_it, players.end(), "Invalid player ID");

	UnitType* unitType = UnitType::getByIdentifier(update->type);
	std::unique_ptr<DrawableUnit> newUnit = std::make_unique<DrawableUnit>(
		*unitType->createUnit(update->id, glm::vec3(update->x, update->y, update->z), player_it->second.get(), nullptr).get(),
		colorShader
	);

	player_it->second->acquire_object(newUnit.get());
	units.insert(std::make_pair(update->id, std::move(newUnit)));
}


void Client::unitSpawnerUpdateHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<UnitSpawnerUpdate> update) {
	auto& spawner_it = this->spawners.find(update->id);
	if (spawner_it == this->spawners.end()) {
		LOG_ERR("Invalid spawner id ", update->id);
		return;
	}

	LOG_DEBUG(update->percent);
	update->apply(spawner_it->second);
}

void Client::cityCreationUpdateHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<CityCreationUpdate> update) {
	LOG_DEBUG("City creation update received");
	auto& player_iter = players.find(update->player_id);
	Lib::assertTrue(player_iter != players.end(), "Invalid player ID");

	auto& slot_iter = slots.find(update->slot_id);
	Lib::assertTrue(slot_iter != slots.end(), "Invalid slot id");

	Player* owner = player_iter->second.get();
	City city(update->city_id, owner, new InstantLaserAttack(), nullptr, update->defense, update->health, update->production, update->population, slot_iter->second, update->name);
	DrawableCity* newCity = new DrawableCity(city, colorShader);
	slot_iter->second->attachCity(newCity);
	owner->acquire_object(newCity);
	cities.insert(std::make_pair(newCity->getID(), newCity));
	spawners.insert(std::make_pair(newCity->getID(), newCity));

	if (newCity->get_player()->getID() == player->getID()) {
		setSelection({ newCity });
	}
}

void Client::playerIdConfirmationHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerIDConfirmation> update) {
	LOG_DEBUG("Received Player ID confirmation -- I am player with id ", update->id);
	std::string player_name = Lib::INIParser::getInstance().get<std::string>("PlayerName");
	this->player = std::make_shared<Player>(player_name, update->id, update->color);
	players[update->id] = this->player;

	gui->setPlayer(this->player);

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
	if (units[update->id]->is_dead()) {
		if (selection.size() > 0 && selection[0]->getID() == update->id) {
			selection.erase(selection.begin());
			units[update->id]->unselect(gui, this);
		}
		units.erase(update->id);
	}
}

void Client::cityUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<CityUpdate> update) {
	update->apply(cities[update->id].get());

	// Handle city death
	LOG_DEBUG("City with ID " + std::to_string(update->id) + " health is " + std::to_string(cities[update->id]->get_health()));
	if (cities[update->id]->is_dead()) {
		Player* owner = cities[update->id]->get_player();
		Slot* slot = cities[update->id]->get_slot();

		slot->detachCity();
		owner->add_to_destroy(cities[update->id].get());
		if (selection.size() > 0 && selection[0]->getID() == update->id) {
			selection.erase(selection.begin());
			cities[update->id]->unselect(gui, this);
		}
		cities.erase(update->id);
	}
}

void Client::planetUpdateHandler(SunNet::ChanneledSocketConnection_p socketConnection, std::shared_ptr<PlanetUpdate> update) {
	DrawablePlanet* planet = planets[update->id].get();
	update->apply(planet);
	planet->update();
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

void Client::slotUpdateHandler(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<SlotUpdate> update) {
	LOG_DEBUG("Received a slot update");
	auto& slot_it = slots.find(update->id);
	Lib::assertNotEqual(slot_it, slots.end(), "Slot for update not found");

	update->apply(slot_it->second);
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

