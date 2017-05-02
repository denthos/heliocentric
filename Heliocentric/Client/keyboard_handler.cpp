#include "keyboard_handler.h"

void KeyboardHandler::callHandler(KeyHandlerMap& handlers, int key) {
	auto& handler_it = handlers.find(key);
	if (handler_it != handlers.end()) {
		for (auto& func : handler_it->second) {
			func(key);
		}
	}
}

void KeyboardHandler::callKeyboardHandlers() {
	for (auto& key_handler : down_handlers) {
		if (keys[key_handler.first]) {
			for (auto& handler : key_handler.second) {
				handler(key_handler.first);
			}
		}
	}
}

void KeyboardHandler::setKeyDown(int key) {
	this->keys[key] = true;

	this->callHandler(this->press_handlers, key);
	this->callHandler(this->down_handlers, key);
}

void KeyboardHandler::setKeyUp(int key) {
	this->keys[key] = false;
}

void KeyboardHandler::registerKeyPressHandler(int key, KeyHandler handler) {
	this->press_handlers[key].push_back(handler);
}

void KeyboardHandler::registerKeyPressHandler(std::initializer_list<int> keys, KeyHandler handler) {
	for (int key : keys) {
		this->registerKeyPressHandler(key, handler);
	}
}

void KeyboardHandler::registerKeyDownHandler(int key, KeyHandler handler) {
	this->down_handlers[key].push_back(handler);
}

void KeyboardHandler::registerKeyDownHandler(std::initializer_list<int> keys, KeyHandler handler) {
	for (int key : keys) {
		this->registerKeyDownHandler(key, handler);
	}
}