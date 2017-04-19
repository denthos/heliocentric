#pragma once

#include "identifiable.h"
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
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Client {
public:
	static int width, height;
	static float fov, nearPlane, farPlane;
	static glm::mat4 perspectiveMatrix, viewMatrix;
	static glm::vec3 camPos, camLookAt, camUp;
	static std::string windowTitle;
	static std::unordered_map<UID, Player *> playerMap;
	static std::unordered_map<UID, Planet *> planetMap;
	static std::unordered_map<UID, Unit *> unitMap;
	static std::unordered_map<UID, City *> cityMap;
	static std::unordered_map<UID, Slot *> slotMap;
	//static Octree<GameObject *> octree;

	static GLFWwindow * createWindow(int width, int height);
	static void initialize();
	static void display(GLFWwindow *);
	static void update();
	static void errorCallback(int error, const char * description);
	static void resizeCallback(GLFWwindow *, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow *, int button, int action, int mods);
	static void mouseCursorCallback(GLFWwindow *, double x, double y);
	static void mouseWheelCallback(GLFWwindow *, double x, double y);

	static void clean(); //free all memory here
	
	static void playerUpdateHandler(PlayerUpdate *);
	static void unitUpdateHandler(UnitUpdate *);
	static void cityUpdateHandler(CityUpdate *);
	static void planetUpdateHandler(PlanetUpdate *);
	static void slotUpdateHandler(SlotUpdate *);
};

