#pragma once

#include "identifiable.h"
#include "camera.h"
#include "octree.h"
#include "player.h"
#include "unit.h"
#include "city.h"
#include "planet.h"
#include "slot.h"
#include "player_update.h"
#include "unit_update.h"
#include "city_update.h"
#include "planet_update.h"
#include "slot_update.h"
#include "channeled_client.h"
#include "tcp_socket_connection.hpp"
#include "universe.h"
#include "unit_manager.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Client : public SunNet::ChanneledClient<SunNet::TCPSocketConnection> {
public:	
	Client();
	~Client(); // free all memory here

	Universe universe;
	UnitManager unit_manager;

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
	void unitUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<UnitUpdate>);
	void cityUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<CityUpdate>);
	void planetUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlanetUpdate>);
	void slotUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<SlotUpdate>);

protected:
	/**** Handlers for ChanneledClient ****/
	virtual void handle_client_disconnect();
	virtual void handle_client_error();
	virtual void handle_poll_timeout();

private:
	GLFWwindow * window;
	Camera * camera;
	std::string windowTitle;

	std::unordered_map<UID, Player *> players;
	std::unordered_map<UID, Planet *> planets;
	std::unordered_map<UID, Unit *> units;
	std::unordered_map<UID, City *> cities;
	std::unordered_map<UID, Slot *> slots;
	std::unordered_map<UID, Drawable *> gameObjects;
	Octree octree;

	void createWindow(int width, int height);
};

