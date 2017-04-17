#pragma once

#include "ConfigParser.h"

#include <unordered_map>


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
	using ConfigParser::get_value;

	INIParser(std::string fname) : ConfigParser(fname) { reload(); };
	~INIParser() {};
	virtual bool reload();
	virtual bool get_value(std::string key, std::string& ret);
	virtual void update_value(std::string key, std::string value);

private:
	std::unordered_map<std::string, std::string> settings;
};

