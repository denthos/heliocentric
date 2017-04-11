#ifndef CLIENT_H_
#define CLIENT_H_

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <string>

class Client {
public:
	static int width, height;
	static float fov, nearPlane, farPlane;
	static glm::mat4 perspectiveMatrix, viewMatrix;
	static glm::vec3 camPos, camLookAt, camUp;
	static std::string windowTitle;

	static GLFWwindow * createWindow(int width, int height);
	static void initialize();
	static void display(GLFWwindow *);
	static void update();
	static void errorCallback(int error, const char * description);
	static void resizeCallback(GLFWwindow *, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow *, int button, int action, int mods);
	static void mouseCursorCallback(GLFWwindow *, double x, double y);
	static void mouseWheelCallback(GLFWwindow *, double x, double y);
};





#endif