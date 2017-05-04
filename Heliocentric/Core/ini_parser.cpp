#include "ini_parser.h"
#include "fileutils.h"

#include <unordered_map>

namespace Lib {

	INIParser& INIParser::getInstance(std::string fname) {
		static std::unordered_map<std::string, INIParser> parsers;
		// Functions used here are chosen carefully to avoid requiring
		// public constructors
		if (parsers.find(fname) == parsers.end()) {
			parsers.insert(std::make_pair(fname, INIParser(fname)));
		}
		return parsers.at(fname);
	}

	INIParser& INIParser::getInstance() {
		auto path_pair = Lib::splitIntoParentAndChildPath(Lib::getProcessPath());
		std::string fname = Lib::joinPaths(path_pair.first, Lib::stripExtension(path_pair.second) + "_config.ini");

		return getInstance(fname);
	}

	void INIParser::reload() {
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
		}
		else {
			std::string message("Could not open config file: ");
			message.append(file_name);
			throw std::runtime_error(message);
		}
	}

	std::string INIParser::get(std::string key) {
		std::unordered_map<std::string, std::string>::iterator iter = settings.find(key);

		if (iter != settings.end()) {
			return iter->second;
		}
		else {
			std::string message("Could not find value for key: ");
			message.append(key);
			message.append(", in file: ");
			message.append(file_name);
			throw std::runtime_error(message);
		}
	}

	void INIParser::update_value(std::string key, std::string value) {
		settings[key] = value;
	}
}
