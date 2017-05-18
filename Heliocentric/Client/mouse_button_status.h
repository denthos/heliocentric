#pragma once

#include <ctime>

struct MouseButtonStatus {
	bool down;
	clock_t lastClick;
	clock_t lastDown;
};