#include "orbital_camera.h"

#include <glm\gtc\matrix_transform.hpp>

#define RESET_KEY "ResetKey"

inline int toLower(int c) {
	return c >= 97 && c <= 122 ? c - 32 : c;
}

OrbitalCamera::OrbitalCamera(KeyboardHandler & keyboardHandler, MouseHandler & mouseHandler, const std::vector<GameObject *> & selection) : selection(selection) {
	loadSettings(keyboardHandler);
	mouseHandler.registerMouseCursorHandler(std::bind(&OrbitalCamera::handleCursorInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	mouseHandler.registerMouseWheelHandler(std::bind(&OrbitalCamera::handleWheelInput, this, std::placeholders::_1));
}

void OrbitalCamera::update() {
	if (selection.size() == 1) {
		glm::vec3 oldTarget = this->target;
		this->target = selection[0]->get_position();
		glm::vec3 translate = this->target - oldTarget;
		this->position += translate;
	}
}

void OrbitalCamera::loadSettings(KeyboardHandler & keyboardHandler, Lib::INIParser & config) {
	resetKey = toLower((int)config.get<std::string>(RESET_KEY)[0]);

	keyboardHandler.registerKeyDownHandler(resetKey, std::bind(&OrbitalCamera::handleReset, this, std::placeholders::_1));
	// load other settings
	this->speed = config.get<float>(CAMERA_SPEED_KEY);
	this->sensitivity = config.get<float>(CAMERA_SENSITIVITY_KEY);
}

void OrbitalCamera::handleCursorInput(const MouseButtonMap & mouseButtons, ScreenPosition currPosition, ScreenPosition lastPosition) {
	if (mouseButtons.at(MouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_NONE)).down) {
		glm::vec3 camFocus = position - target;
		glm::vec3 right = glm::cross(up, glm::normalize(camFocus));
		float angle = (lastPosition.second - currPosition.second) / -100.0f;
		position = glm::vec3(glm::vec4(camFocus, 1.0f) * glm::rotate(glm::mat4(1.0f), angle, right)) + target;
		camFocus = position - target;
		angle = (lastPosition.first - currPosition.first) / -100.0f;
		position = glm::vec3(glm::vec4(camFocus, 1.0f) * glm::rotate(glm::mat4(1.0f), angle, up)) + target;
	}
}

void OrbitalCamera::handleWheelInput(ScreenPosition input) {
	glm::vec3 translate = (target - position) * this->speed * input.second * 0.01f;
	this->position += translate;
}

void OrbitalCamera::handleReset(int key) {
	if (key == resetKey) {
		this->target = glm::vec3(0.0f);
		this->position = glm::vec3(0.0f, 0.0f, 250.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}