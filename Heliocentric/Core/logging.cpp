#include "logging.h"
#include "fileutils.h"

namespace Lib {
	
	std::ofstream& getLogFile() {
		static std::ofstream log_file;
		if (!log_file.is_open()) {
			auto path_pair = Lib::splitIntoParentAndChildPath(Lib::getProcessPath());
			std::string log_path = Lib::joinPaths(path_pair.first, Lib::stripExtension(path_pair.second) + ".log");
			log_file.open(log_path, std::ios::app);
		}
		return log_file;
	}

	void LogFuncRecursive(int level, const char* file, const char* ext, const char* dir, int line, const char* time_str, std::ostringstream& stream) {
		std::string level_string;
		switch (level) {
		case LOG_LEVEL_DEBUG:
			level_string = "DEBUG";
			break;
		case LOG_LEVEL_INFO:
			level_string = "INFO";
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

		{
			std::lock_guard<std::mutex> log_guard(log_lock);
			write_log_to_stream(std::cerr, level_string, file, ext, line, time_str, stream);
			write_log_to_stream(getLogFile(), level_string, file, ext, line, time_str, stream);
		}
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