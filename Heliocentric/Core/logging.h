#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iomanip> 
#include <time.h>

#ifndef _WIN32
#include <libgen.h>
#endif

#include "ini_parser.h"

namespace Lib {
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_ERR 2

#define LOG(level, ...) LogFunc(level, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_WARN(...) LOG(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_ERR(...) LOG(LOG_LEVEL_ERR, __VA_ARGS__)

	std::ofstream& getLogFile(std::string dir) {
		static std::ofstream log_file;
		if (!log_file.is_open()) {
			log_file.open(dir + "/log.log", std::ios::app);
		}
		return log_file;
	}

	template <typename TStreamType>
	void write_log_to_stream(TStreamType& out_stream, std::string& level_string, const char* file, const char* ext, int line, const char* time_str, std::ostringstream& stream) {
		out_stream << time_str << " " << file << ext << "(" << line << ") [" << level_string << "] " << stream.str() << std::endl;
	}

	template <typename... TLogItems>
	void LogFunc(int level, const char* file, int line, const TLogItems&... args) {
		int log_level;
		INIParser::getInstance().get_value("loglevel", log_level);
		if (level < log_level) {
			return;
		}

		char time_str[20];
		const time_t current_gmt_time = time(NULL);
		struct tm local_time;
		localtime_s(&local_time, &current_gmt_time);
		strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", &local_time);

		char file_extension[_MAX_EXT];
		char file_name[_MAX_FNAME];
		char file_dir[_MAX_DIR];
		get_file_info(file, file_name, _MAX_FNAME, file_extension, _MAX_EXT, file_dir, _MAX_DIR);

		std::ostringstream log_msg;
		LogFuncRecursive(level, file_name, file_extension, file_dir, line, time_str, log_msg, args...);
	}

	template <typename TLogItem, typename... TRemainingLogItems>
	void LogFuncRecursive(int level, const char* file, const char* ext, const char* dir, int line, const char* time_str, std::ostringstream& stream,
		TLogItem value, const TRemainingLogItems&... args) {

		stream << value;
		LogFuncRecursive(level, file, ext, dir, line, time_str, stream, args...);
	}

	void LogFuncRecursive(int level, const char* file, const char* ext, const char* dir, int line, const char* time_str, std::ostringstream& stream) {
		std::string level_string;
		switch (level) {
		case LOG_LEVEL_DEBUG:
			level_string = "DEBUG";
			break;
		case LOG_LEVEL_WARN:
			level_string = "WARN";
			break;
		case LOG_LEVEL_ERR:
			level_string = "ERROR";
			break;
		default:
			level_string = "LOG";
		}

		write_log_to_stream(std::cerr, level_string, file, ext, line, time_str, stream);
		write_log_to_stream(getLogFile(dir), level_string, file, ext, line, time_str, stream);
	}



	void get_file_info(const char* path, char* filename, int filename_size, char* extension, int ext_size, char* directory, int dir_size) {
#ifdef _WIN32
		_splitpath_s(path, NULL, 0, directory, dir_size, filename, filename_size, extension, ext_size);
#else
		dirname_r(path, directory);
		basename_r(path, filename);
		memset(extension, 0, ext_size);
#endif
	}
}