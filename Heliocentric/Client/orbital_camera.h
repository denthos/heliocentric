#pragma once

#include "camera.h"

class OrbitalCamera : public Camera {
public:
	virtual void update();
	virtual void loadSettings(Lib::INIParser & config = Lib::INIParser::getInstance());
};