#pragma once

#include <unordered_map>
#include <functional>

#define MAX_KEYS 1024

class KeyboardHandler {
	typedef std::function<void(int)> KeyHandler;
	typedef std::unordered_map<int, std::vector<KeyHandler>> KeyHandlerMap;


private:
	KeyHandlerMap press_handlers;
	KeyHandlerMap down_handlers;
	bool keys[MAX_KEYS];

	void callHandler(KeyHandlerMap& handlers, int key);

public:

	void keyCallback(int key, int scancode, int action, int mods);

	void callKeyboardHandlers();

	void setKeyDown(int key);
	void setKeyUp(int key);


	void registerKeyPressHandler(int key, KeyHandler handler);
	void registerKeyPressHandler(std::initializer_list<int> keys, KeyHandler handler);

	void registerKeyDownHandler(int key, KeyHandler handler);
	void registerKeyDownHandler(std::initializer_list<int> keys, KeyHandler handler);
};