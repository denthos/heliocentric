#include "INIParser.h"

#include <fstream>
#include <iterator>

bool INIParser::reload() {
	std::ifstream myfile;
	myfile.open(file_name.c_str());

	if (myfile.is_open()) {
		std::string line;
		while (myfile.good()) {
			std::getline(myfile, line);
			size_t pos = line.find_first_of(':');

			if (pos > 0 && pos < line.length()) {
				std::string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);

				update_value(key, value);
			}
		}
		myfile.close();
		return true;
	}

	return false;
}

bool INIParser::get_value(std::string key, std::string& ret) {
	std::unordered_map<std::string, std::string>::iterator iter = settings.find(key);

	if (iter != settings.end()) {
		ret = iter->second;
		return true;
	}

	return false;
}

void INIParser::update_value(std::string key, std::string value) {
	settings[key] = value;
}
