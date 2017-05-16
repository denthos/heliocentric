#pragma once

#include "camera.h"
#include "keyboard_handler.h"
#include "mouse_handler.h"
#include "lib.h"

class FreeCamera : public Camera {
public:
	FreeCamera(KeyboardHandler &, MouseHandler &);
	virtual void update();
	virtual void loadSettings(KeyboardHandler &, Lib::INIParser & config = Lib::INIParser::getInstance());
private:
	int forwardKey, backwardKey, leftKey, rightKey, upKey, downKey;
	float speed, sensitivity, invertYAxis;
	float yaw, pitch;
	void handleKeyInput(int);
	void handleCursorInput(const MouseButtonMap &, ScreenPosition, ScreenPosition);
	void handleWheelInput(ScreenPosition);
};