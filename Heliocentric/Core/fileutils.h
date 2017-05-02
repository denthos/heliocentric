#pragma once

#include <string>
#include <vector>


namespace Lib {
	void getFilesInDir(const std::string& directory, std::vector<std::string>& out, 
		bool recursive = false, bool relative = true, std::string base_dir = "");

	std::string stripExtension(const std::string& path);
}