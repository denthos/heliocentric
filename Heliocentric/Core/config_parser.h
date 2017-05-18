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
		virtual void reload() = 0;

		/* getters
		*
		* NOTE: the int, float, and double getters will load 0 when the
		key exists but its associated string value could not be
		converted to the appropriate type.
		For example, if the key "ScreenWidth" is set to "hello, world",
		`get_value("ScreenWidth", ret)` will set `ret` to 0.
		*/

		template<typename T>
		T get(std::string key);

		template<>
		std::string get(std::string key) {
			return this->get(key);
		}

		template<>
		bool get(std::string key) {
			std::string value = this->get(key);
			return (value == "true") ? true : false;
		}

		template<>
		int get(std::string key) {
			std::string value = this->get(key);
			return std::atoi(value.c_str());
		}

		template<>
		char get(std::string key) {
			char val = this->get(key)[0];
			return val >= 97 && val <= 122 ? val - 32 : val;
		}

		template<>
		float get(std::string key) {
			std::string value = this->get(key);
			return (float)std::atof(value.c_str());
		}

		template<>
		double get(std::string key) {
			std::string value = this->get(key);
			return std::atof(value.c_str());
		}

		/* setters */
		virtual void update_value(std::string key, std::string value) = 0;
		void update_value(std::string key, bool value);
		void update_value(std::string key, int value);
		void update_value(std::string key, float value);
		void update_value(std::string key, double value);


	protected:
		virtual std::string get(std::string) = 0;
		std::string file_name;
	};
}