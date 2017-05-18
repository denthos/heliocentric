#pragma once

#include "camera.h"
#include "game_object.h"
#include "keyboard_handler.h"
#include "mouse_handler.h"

class OrbitalCamera : public Camera {
public:
	OrbitalCamera(KeyboardHandler &, MouseHandler &, const std::vector<GameObject *> &);
	virtual void update();
	virtual void loadSettings(KeyboardHandler &, Lib::INIParser & config = Lib::INIParser::getInstance());
private:
	int resetKey;
	float speed, sensitivity;
	const std::vector<GameObject *> & selection;

	void handleCursorInput(const MouseButtonMap &, ScreenPosition, ScreenPosition);
	void handleWheelInput(ScreenPosition);
	void handleReset(int);
};