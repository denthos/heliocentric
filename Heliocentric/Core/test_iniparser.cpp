#include "INIParser.h"
#include <iostream>
#include <string>
#include <cassert>


int main() {
	/* Load config file */
	INIParser config("ConfigTemplate.ini");

	/* Retrieve an int from the config file. */ 
	int value = 600;  // Set 600 as the default.
	bool exists = config.get_value("ScreenWidth", value);
	assert(exists && value==800);
}