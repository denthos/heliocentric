#include "free_camera.h"

#include <glm\gtc\matrix_transform.hpp>

#define FORWARD_KEY "ForwardKey"
#define BACKWARD_KEY "BackwardKey"
#define LEFT_KEY "LeftKey"
#define RIGHT_KEY "RightKey"
#define UP_KEY "UpKey"
#define DOWN_KEY "DownKey"
#define RESET_KEY "ResetKey"

#define DEFAULT_POSITION glm::vec3(0.0f, 0.0f, 1000.0f)
#define DEFAULT_TARGET glm::vec3(0.0f, 0.0f, 0.0f)
#define DEFAULT_UP glm::vec3(0.0f, 1.0f, 0.0f)

#define CAMERA_SPEED "CameraSpeed"
#define CAMERA_SENSITIVITY "CameraSensitivity"
#define INVERT_Y_AXIS "InvertYAxis"

FreeCamera::FreeCamera(KeyboardHandler & keyboardHandler, MouseHandler & mouseHandler) {
	loadSettings(keyboardHandler);
	yaw = -90.0f;
	pitch = 0.0f;
	mouseHandler.registerMouseCursorHandler(std::bind(&FreeCamera::handleCursorInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	mouseHandler.registerMouseWheelHandler(std::bind(&FreeCamera::handleWheelInput, this, std::placeholders::_1));
}

void FreeCamera::update() {

}

void FreeCamera::handleKeyInput(int key) {
	if (!active) return;
	// can't use member variables in switch statements
	//   so we use a long if chain
	if (key == forwardKey) {
		glm::vec3 translation = glm::normalize(target - position) * speed;
		position += translation;
		target += translation;
	}
	else if (key == backwardKey) {
		glm::vec3 translation = glm::normalize(target - position) * speed;
		position -= translation;
		target -= translation;
	}
	else if (key == leftKey) {
		glm::vec3 translation = glm::normalize(glm::cross(target - position, up)) * speed;
		position -= translation;
		target -= translation;
	}
	else if (key == rightKey) {
		glm::vec3 translation = glm::normalize(glm::cross(target - position, up)) * speed;
		position += translation;
		target += translation;
	}
	else if (key == upKey) {
		glm::vec3 translation = up * speed;
		position += translation;
		target += translation;
	}
	else if (key == downKey) {
		glm::vec3 translation = up * speed;
		position -= translation;
		target -= translation;
	}
	else if (key == resetKey) {
		position = DEFAULT_POSITION;
		target = DEFAULT_TARGET;
		up = DEFAULT_UP;
		yaw = -90.0f;
		pitch = 0.0f;
	}
}

void FreeCamera::loadSettings(KeyboardHandler & keyboardHandler, Lib::INIParser & config) {
	
	// load key bindings
	forwardKey = (int)config.get<char>(FORWARD_KEY);
	backwardKey = (int)config.get<char>(BACKWARD_KEY);
	leftKey = (int)config.get<char>(LEFT_KEY);
	rightKey = (int)config.get<char>(RIGHT_KEY);
	upKey = (int)config.get<char>(UP_KEY);
	downKey = (int)config.get<char>(DOWN_KEY);
	resetKey = (int)config.get<char>(RESET_KEY);

	keyboardHandler.registerKeyDownHandler({ forwardKey, backwardKey, leftKey, rightKey, upKey, downKey, resetKey }, std::bind(&FreeCamera::handleKeyInput, this, std::placeholders::_1));

	// load other settings
	this->speed = config.get<float>(CAMERA_SPEED);
	this->sensitivity = config.get<float>(CAMERA_SENSITIVITY);
	this->invertYAxis = config.get<bool>(INVERT_Y_AXIS) ? -1.0f : 1.0f;
}

void FreeCamera::handleCursorInput(const MouseButtonMap & mouseButtons, ScreenPosition currPosition, ScreenPosition lastPosition) {
	if (!active) return;
	if (mouseButtons.at(MouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_NONE)).down) {
		float xoffset = (currPosition.first - lastPosition.first) * sensitivity;
		float yoffset = (currPosition.second - lastPosition.second) * sensitivity * invertYAxis;
		yaw += xoffset;
		pitch -= yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		float yawRads = glm::radians(yaw);
		float pitchRads = glm::radians(pitch);
		glm::vec3 front = glm::normalize(glm::vec3(
			glm::cos(yawRads) * glm::cos(pitchRads),
			glm::sin(pitchRads),
			glm::sin(yawRads) * glm::cos(pitchRads)
		));
		target = position + front;
	}
}

void FreeCamera::handleWheelInput(ScreenPosition input) {
	if (!active) return;
	fov -= input.second * sensitivity;
	if (fov <= 44.5f)
		fov = 44.5f;
	if (fov >= 45.0f)
		fov = 45.0f;
	this->calculatePerspectiveMatrix();
	this->calculateInfinitePerspectiveMatrix();
}