#pragma once

#include "game_object.h"

#include <nanogui\nanogui.h>

#if defined(NANOGUI_GLAD)
	#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
		#define GLAD_GLAPI_EXPORT
	#endif

	#include <glad/glad.h>
#else
	#if defined(__APPLE__)
		#define GLFW_INCLUDE_GLCOREARB
	#else
		#define GL_GLEXT_PROTOTYPES
	#endif
#endif

using namespace nanogui;

class GUI : public Screen {
public:
	GUI(GLFWwindow *);
	void updateSelection(GameObject * selected);
	void show();
	void hide();
	bool isHidden();

protected:
	ref<Window> userDisplay;
	FormHelper * gui;
	bool hidden;
private:
	
	detail::FormWidget<int> * uidDisplay;
};