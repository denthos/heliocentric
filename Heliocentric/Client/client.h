#pragma once

#include "identifiable.h"
#include "camera.h"
#include "octree.h"
#include "player.h"
#include "unit.h"
#include "drawable_unit.h"
#include "city.h"
#include "drawable_city.h"
#include "planet.h"
#include "drawable_planet.h"
#include "slot.h"
#include "drawable_slot.h"
#include "player_update.h"
#include "unit_creation_update.h"
#include "unit_update.h"
#include "city_update.h"
#include "planet_update.h"
#include "player_id_confirmation.h"
#include "channeled_client.h"
#include "tcp_socket_connection.hpp"
#include "universe.h"
#include "unit_manager.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include "keyboard_handler.h"
#include "locked_item.h"

class Client : public SunNet::ChanneledClient<SunNet::TCPSocketConnection> {
public:	
	Client();
	~Client(); // free all memory here

	Universe universe;
	UnitManager unit_manager;
	KeyboardHandler keyboard_handler;

	bool isRunning();
	void display();
	void update();
	void errorCallback(int error, const char * description);
	void resizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);
	void mouseButtonCallback(int button, int action, int mods);
	void mouseCursorCallback(double x, double y);
	void mouseWheelCallback(double x, double y);
	
	void playerUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerUpdate>);
	void unitCreationUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<UnitCreationUpdate>);
	void unitUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<UnitUpdate>);
	void cityUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<CityUpdate>);
	void planetUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlanetUpdate>);
	void slotUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<SlotUpdate>);
	void playerIdConfirmationHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerIDConfirmation>);

protected:
	/**** Handlers for ChanneledClient ****/
	virtual void handle_client_disconnect();
	virtual void handle_client_error();
	virtual void handle_poll_timeout();

private:
	GLFWwindow * window;
	Camera * camera;
	std::string windowTitle;

	std::unordered_map<UID, std::shared_ptr<Player>> players;
	std::unordered_map<UID, std::unique_ptr<DrawablePlanet>> planets;
	std::unordered_map<UID, std::unique_ptr<DrawableUnit>> units;
	std::unordered_map<UID, std::unique_ptr<DrawableCity>> cities;
	std::unordered_map<UID, std::unique_ptr<DrawableSlot>> slots;
	std::unordered_map<UID, std::unique_ptr<Drawable>> dead_objects;
	Octree octree;

	std::shared_ptr<Player> player;

	Lib::Lock<std::queue<std::function<void()>>> update_queue;

	void createWindow(int width, int height);

	void handleEscapeKey(int);
	void handleCameraPanButtonDown(int);
	void handleF1Key(int);
	void handleF2Key(int);
	void handleF3Key(int);
	void handleF4Key(int);
	void handleF5Key(int);
	void handleF6Key(int);
};

