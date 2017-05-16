#include "mouse_handler.h"

#include <ctime>
#include <GLFW\glfw3.h>

#define CLICK_DELAY "ClickDelay"
#define DOUBLE_CLICK_DELAY "DoubleClickDelay"

MouseHandler::MouseHandler() {
	loadSettings();
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
		for (int j = 0; j < (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT | GLFW_MOD_SUPER); ++j) {
			mouseButtons[MouseButton(i, j)] = { false, 0, 0 };
		}
	}
	cursorPosition = std::make_pair(-1.0f, -1.0f);
}

void MouseHandler::loadSettings(Lib::INIParser & config) {
	clickDelay = (clock_t)config.get<int>(CLICK_DELAY);
	doubleClickDelay = (clock_t)config.get<int>(DOUBLE_CLICK_DELAY);
}

void MouseHandler::mouseButtonCallback(int button, int action, int mods) {
	clock_t time = std::clock();
	MouseButton mouseButton = MouseButton(button, mods);

	if (action == GLFW_PRESS) {
		mouseButtons[mouseButton].down = true;
		mouseButtons[mouseButton].lastDown = time;
		// fire mouse down callbacks
		for (auto & func : pressHandlers[mouseButton]) {
			func(mouseButton, cursorPosition);
		}
	}
	else if (action == GLFW_RELEASE) {
		mouseButtons[mouseButton].down = false;
		// fire mouse release callbacks
		if (time - mouseButtons[mouseButton].lastDown <= clickDelay) {
			// fire mouse click callbacks
			if (time - mouseButtons[mouseButton].lastClick <= doubleClickDelay) {
				for (auto & func : doubleClickHandlers[mouseButton]) {
					func(mouseButton, cursorPosition);
				}
				mouseButtons[mouseButton].lastClick = 0;
			}
			else {
				for (auto & func : clickHandlers[mouseButton]) {
					func(mouseButton, cursorPosition);
				}
				mouseButtons[mouseButton].lastClick = time;
			}
		}
		else {
			for (auto & func : releaseHandlers[mouseButton]) {
				func(mouseButton, cursorPosition);
			}
		}
	}
}

void MouseHandler::mouseCursorCallback(double x, double y) {
	ScreenPosition currPosition = ScreenPosition((float)x, (float)y);
	if (cursorPosition.first < 0.0f)
		cursorPosition = currPosition;
	for (auto & func : cursorHandlers) {
		func(mouseButtons, currPosition, this->cursorPosition);
	}
	this->cursorPosition = currPosition;
}

void MouseHandler::mouseWheelCallback(double x, double y) {
	for (auto & func : wheelHandlers) {
		func(ScreenPosition((float)x, (float)y));
	}
}

void MouseHandler::registerMousePressHandler(MouseButton mouseButton, MouseButtonHandler handler) {
	this->pressHandlers[mouseButton].push_back(handler);
}
void MouseHandler::registerMousePressHandler(std::initializer_list<MouseButton> mouseButtons, MouseButtonHandler handler) {
	for (MouseButton mouseButton : mouseButtons) {
		this->registerMousePressHandler(mouseButton, handler);
	}
}

void MouseHandler::registerMouseReleaseHandler(MouseButton mouseButton, MouseButtonHandler handler) {
	this->releaseHandlers[mouseButton].push_back(handler);
}
void MouseHandler::registerMouseReleaseHandler(std::initializer_list<MouseButton> mouseButtons, MouseButtonHandler handler) {
	for (MouseButton mouseButton : mouseButtons) {
		this->registerMouseReleaseHandler(mouseButton, handler);
	}
}

void MouseHandler::registerMouseClickHandler(MouseButton mouseButton, MouseButtonHandler handler) {
	this->clickHandlers[mouseButton].push_back(handler);
}
void MouseHandler::registerMouseClickHandler(std::initializer_list<MouseButton> mouseButtons, MouseButtonHandler handler) {
	for (MouseButton mouseButton : mouseButtons) {
		this->registerMouseClickHandler(mouseButton, handler);
	}
}

void MouseHandler::registerMouseDoubleClickHandler(MouseButton mouseButton, MouseButtonHandler handler) {
	this->doubleClickHandlers[mouseButton].push_back(handler);
}
void MouseHandler::registerMouseDoubleClickHandler(std::initializer_list<MouseButton> mouseButtons, MouseButtonHandler handler) {
	for (MouseButton mouseButton : mouseButtons) {
		this->registerMouseDoubleClickHandler(mouseButton, handler);
	}
}

void MouseHandler::registerMouseCursorHandler(MouseCursorHandler handler) {
	cursorHandlers.push_back(handler);
}

void MouseHandler::registerMouseWheelHandler(MouseWheelHandler handler) {
	wheelHandlers.push_back(handler);
}

MouseButtonStatus MouseHandler::getMouseButtonStatus(int button, int mods) {
	return getMouseButtonStatus(MouseButton(button, mods));
}

MouseButtonStatus MouseHandler::getMouseButtonStatus(MouseButton mouseButton) {
	return mouseButtons[mouseButton];
}