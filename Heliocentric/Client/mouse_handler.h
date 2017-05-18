#pragma once

#include "lib.h"
#include "mouse_button_status.h"
#include <functional>

// this include isn't actually required here, but we want anything that
//   gets our new define to also have all of the regular key definitions
#include "GLFW\glfw3.h"
#define GLFW_MOD_NONE 0

typedef std::pair<int, int> MouseButton;
typedef std::pair<float, float> ScreenPosition;

struct MouseButtonHasher {
	std::size_t operator()(const MouseButton & key) const {
		return std::hash<int>()((key.first << 4) | (key.second & 15));
	}
};

typedef std::unordered_map<MouseButton, MouseButtonStatus, MouseButtonHasher> MouseButtonMap;

class MouseHandler {

	typedef std::function<void(const MouseButtonMap &, ScreenPosition, ScreenPosition)> MouseCursorHandler;
	typedef std::function<void(ScreenPosition)> MouseWheelHandler;
	typedef std::function<void(MouseButton, ScreenPosition)> MouseButtonHandler;
	
	typedef std::unordered_map<MouseButton, std::vector<MouseButtonHandler>, MouseButtonHasher> MouseButtonHandlerMap;
public:
	MouseHandler();
	
	void loadSettings(Lib::INIParser & config = Lib::INIParser::getInstance());

	void mouseButtonCallback(int button, int action, int mods);
	void mouseCursorCallback(double x, double y);
	void mouseWheelCallback(double x, double y);

	void registerMousePressHandler(MouseButton, MouseButtonHandler);
	void registerMousePressHandler(std::initializer_list<MouseButton>, MouseButtonHandler);

	void registerMouseReleaseHandler(MouseButton, MouseButtonHandler);
	void registerMouseReleaseHandler(std::initializer_list<MouseButton>, MouseButtonHandler);

	void registerMouseClickHandler(MouseButton, MouseButtonHandler);
	void registerMouseClickHandler(std::initializer_list<MouseButton>, MouseButtonHandler);

	void registerMouseDoubleClickHandler(MouseButton, MouseButtonHandler);
	void registerMouseDoubleClickHandler(std::initializer_list<MouseButton>, MouseButtonHandler);

	void registerMouseCursorHandler(MouseCursorHandler);
	
	void registerMouseWheelHandler(MouseWheelHandler);

	MouseButtonStatus getMouseButtonStatus(int, int);
	MouseButtonStatus getMouseButtonStatus(MouseButton);
private:
	clock_t clickDelay, doubleClickDelay;
	MouseButtonMap mouseButtons;
	ScreenPosition cursorPosition;

	MouseButtonHandlerMap pressHandlers;
	MouseButtonHandlerMap releaseHandlers;
	MouseButtonHandlerMap clickHandlers;
	MouseButtonHandlerMap doubleClickHandlers;
	std::vector<MouseCursorHandler> cursorHandlers;
	std::vector<MouseWheelHandler> wheelHandlers;
};