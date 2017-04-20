#pragma once
#include <sstream>
#include <string>

namespace Lib {
	/*
	This is the base configuration parsers.
	Subclassing it requires defining three methods:
	- bool reload()
	- bool get_value(string key, string& ret)
	- bool update_value(string key, string value);
	*/
	class ConfigParser
	{

	public:
		ConfigParser(std::string fname) : file_name(fname) {};
		~ConfigParser() {};

		/* Handles loading for an expected configuration format. */
		virtual bool reload() = 0;

		/* getters
		*
		* NOTE: the int, float, and double getters will load 0 when the
		key exists but its associated string value could not be
		converted to the appropriate type.
		For example, if the key "ScreenWidth" is set to "hello, world",
		`get_value("ScreenWidth", ret)` will set `ret` to 0.
		*/
		virtual bool get_value(std::string key, std::string& ret) = 0;
		bool get_value(std::string key, bool& ret);
		bool get_value(std::string key, int& ret);
		bool get_value(std::string key, float& ret);
		bool get_value(std::string key, double& ret);

		/* setters */
		virtual void update_value(std::string key, std::string value) = 0;
		void update_value(std::string key, bool value);
		void update_value(std::string key, int value);
		void update_value(std::string key, float value);
		void update_value(std::string key, double value);


	protected:
		std::string file_name;
	};
}