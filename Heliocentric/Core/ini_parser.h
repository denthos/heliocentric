#pragma once
#include "config_parser.h"
#include <fstream>
#include <iterator>
#include <unordered_map>

namespace Lib {
	/*
	An simple INI parser.
	INI files have the format:
	KEY_1: VALUE_1
	KEY_2: VALUE_2
	...
	KEY_N: VALUE_N
	For an example, see "ConfigTemplate.ini"
	*/
	class INIParser : public ConfigParser
	{
	public:
		using ConfigParser::get;

		static INIParser& getInstance(std::string fname = "../config.ini");

		virtual void reload();
		virtual void update_value(std::string key, std::string value);
	protected:
		virtual std::string get(std::string key);
	private:
		INIParser(std::string fname) : ConfigParser(fname) { reload(); };
		std::unordered_map<std::string, std::string> settings;
	};
}
