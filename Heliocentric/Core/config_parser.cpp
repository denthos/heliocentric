#include "config_parser.h"

namespace Lib {

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
}