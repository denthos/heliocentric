#pragma once

#include <string>
#include <vector>
#include <iostream>

#ifdef _WIN32
#define FILE_SEP_CHAR '\\'
#else
#define FILE_SEP_CHAR '/'
#endif


namespace Lib {
	void getFilesInDir(const std::string& directory, std::vector<std::string>& out, 
		bool recursive = false, bool relative = true, std::string base_dir = "");

	std::string stripExtension(const std::string& path);

	std::string getProcessPath();
	std::pair<std::string, std::string> splitIntoParentAndChildPath(const std::string& path);

	template <typename T> 
	std::string joinPaths(T path) {
		return path;
	}

	template <typename T, typename... Args>
	std::string joinPaths(T path, Args... paths) {
		return path + FILE_SEP_CHAR + joinPaths(paths...);
	}
}