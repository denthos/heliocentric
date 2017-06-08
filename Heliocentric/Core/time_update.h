#pragma once
#include <ctime>

class TimeUpdate {
public:
	TimeUpdate(std::clock_t time);

private:
	std::clock_t time;
};