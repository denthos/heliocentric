#include "ConfigParser.h"
#include <sstream>


bool ConfigParser::get_value(std::string key, bool& ret) {
	std::string value;
	bool exists = this->get_value(key, value);
	if (exists) {
		return (value == "true") ? true : false;
	}
	return exists;
}

bool ConfigParser::get_value(std::string key, int& ret) {
	std::string value;
	bool exists = this->get_value(key, value);
	if (exists) {
		ret = std::atoi(value.c_str());
	}
	return exists;
}

bool ConfigParser::get_value(std::string key, float& ret) {
	std::string value;
	bool exists = this->get_value(key, value);
	if (exists) {
		ret = (float) std::atof(value.c_str());
	}
	return exists;
}

bool ConfigParser::get_value(std::string key, double& ret) {
	std::string value;
	bool exists = this->get_value(key, value);
	if (exists) {
		ret = std::atof(value.c_str());
	}
	return exists;
}

void ConfigParser::update_value(std::string key, bool value) {
	std::string v = (value) ? "true" : "false";
	update_value(key, v);
}

void ConfigParser::update_value(std::string key, int value) {
	std::stringstream s;
	s << value;
	update_value(key, s.str());
}

void ConfigParser::update_value(std::string key, float value) {
	std::stringstream s;
	s << value;
	update_value(key, s.str());
}

void ConfigParser::update_value(std::string key, double value) {
	std::stringstream s;
	s << value;
	update_value(key, s.str());
}