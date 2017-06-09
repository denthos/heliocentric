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
#include "new_player_info_update.h"
#include "unit_creation_update.h"
#include "unit_update.h"
#include "city_update.h"
#include "planet_update.h"
#include "player_id_confirmation.h"
#include "channeled_client.h"
#include "tcp_socket_connection.hpp"
#include "universe.h"
#include "unit_manager.h"
#include "music_player.h"
#include "gui.h"
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include "keyboard_handler.h"
#include "mouse_handler.h"
#include "city_creation_update.h"
#include "locked_item.h"
#include "trade_data.h"
#include "trade_deal.h"
#include "selectable.h"
#include "slot_update.h"
#include "game_over_update.h"
#include "player_score_update.h"
#include "player_research_update.h"
#include "threed_sound_system.h"
#include "tech_tree.h"
#include "time_update.h"

class Client : public SunNet::ChanneledClient<SunNet::TCPSocketConnection> {
public:
	static std::unordered_map<GLFWwindow *, Client *> glfwEntry;

	Client();
	~Client(); // free all memory here

	Universe universe;
	UnitManager unit_manager;

	bool isRunning();
	void display();
	void update();

	void mouseClickHandler(MouseButton, ScreenPosition);
	void mouseRightClickHandler(MouseButton, ScreenPosition);
	
	void playerUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerUpdate>);
	void newPlayerInfoUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<NewPlayerInfoUpdate>);
	void unitCreationUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<UnitCreationUpdate>);
	void unitUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<UnitUpdate>);
	void cityUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<CityUpdate>);
	void planetUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlanetUpdate>);
	void playerIdConfirmationHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerIDConfirmation>);
	void playerScoreUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerScoreUpdate>);
	void playerResearchUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerResearchUpdate>);
	void tradeDataHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<TradeData>);
	void cityCreationUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<CityCreationUpdate>);
	void slotUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<SlotUpdate>);
	void gameOverUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<GameOverUpdate>);
	void unitSpawnerUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<SpawnerUpdate>);
	void timeUpdateHandler(SunNet::ChanneledSocketConnection_p, std::shared_ptr<TimeUpdate>);

	void createCityForSlot(DrawableSlot*, std::string);

	void setSelection(std::vector<GameObject*>);
	void createUnitFromCity(DrawableCity* city, Buildable* unit_type);
	void beginResearchOnTechnology(const Technology* tech);
	void sendTradeDeal(std::shared_ptr<TradeData> deal);
	void sendTradeCommand(UID trade_id, bool is_accepted);

protected:
	/**** Handlers for ChanneledClient ****/
	virtual void handle_client_disconnect();
	virtual void handle_client_error();
	virtual void handle_poll_timeout();

private:
	std::thread sound_thread;
	int width, height;

	float exposure;
	GLFWwindow * window;
	GUI * gui;
	ThreeDSoundSystem* soundSystem;
	MusicPlayer* musicPlayer;
	unsigned int selectedCamera;
	std::vector<Camera *> cameras;
	std::vector<GameObject *> selection;
	std::string windowTitle;
	bool init = false;
	double frameTimer;
	unsigned long frameCounter;
	unsigned long lastFrame;
	bool focused;
	Octree * octree;

	GameObject* getObjectAtCursorPosition(const ScreenPosition& position);

	KeyboardHandler keyboard_handler;
	MouseHandler mouse_handler;

	std::unordered_map<UID, std::shared_ptr<Player>> players;
	std::unordered_map<UID, std::unique_ptr<DrawablePlanet>> planets;
	std::unordered_map<UID, std::unique_ptr<DrawableUnit>> units;
	std::unordered_map<UID, std::unique_ptr<DrawableCity>> cities;
	std::unordered_map<UID, DrawableSlot*> slots;
	std::unordered_map<UID, Builder*> spawners;
	std::unordered_map<UID, std::unique_ptr<DrawableUnit>> dead_units;

	std::shared_ptr<Player> player;

	void createWindow(int width, int height);

	// glfw callbacks
	static void errorCallback(int error, const char * description);
	void resizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);
	void mouseButtonCallback(int button, int action, int mods);
	void mouseCursorCallback(double x, double y);
	void scrollWheelCallback(double x, double y);
	void focusCallback(int focused);

	void handleCameraSwitch(int);
	void handleEscapeKey(int);
	void handleF1Key(int);
	void handleF2Key(int);
	void handleF3Key(int);
	void handleF4Key(int);
	void handleF6Key(int);
	void handleF10Key(int);
	void handleLeftBracketKey(int);
	void handleRightBracketKey(int);
	void handleTKey(int);
	void handleUpKey(int);
	void handleDownKey(int);
};

