#pragma once

#include "identifiable.h"
#include "camera.h"
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
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Client : SunNet::ChanneledClient<SunNet::TCPSocketConnection> {
public:	
	Client();
	~Client(); // free all memory here

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
private:
	GLFWwindow * window;
	Camera * camera;
	std::string windowTitle;
	std::unordered_map<UID, Player *> playerMap;
	std::unordered_map<UID, Planet *> planetMap;
	std::unordered_map<UID, Unit *> unitMap;
	std::unordered_map<UID, City *> cityMap;
	std::unordered_map<UID, Slot *> slotMap;
	//Octree<GameObject *> octree;

	void createWindow(int width, int height);
};

