#include "fileutils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

namespace Lib {

	/**
	Shamelessly copied from
	http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
	*/
	void getFilesInDir(const std::string& directory, std::vector<std::string>& out,
		bool recursive, bool relative, std::string base_dir) {

		if (base_dir.empty()) {
			base_dir = directory + "/";
		}

#ifdef _WIN32
		HANDLE dir;
		WIN32_FIND_DATA file_data;


		dir = FindFirstFile((directory + "/*").c_str(), &file_data);
		if (dir == INVALID_HANDLE_VALUE) {
			return;
		}

		do {
			const std::string file_name = file_data.cFileName;
			const std::string full_file_path = directory + "/" + file_name;
			const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

			/* Windows includes "." and ".." entries. Skip them. */
			if (file_name[0] == '.') {
				continue;
			}

			if (is_directory) {
				if (recursive) {
					getFilesInDir(full_file_path, out, recursive, relative, base_dir);
				}
				else {
					continue;
				}
			}

			if (relative) {
				out.push_back(full_file_path.substr(base_dir.size()));
			}
			else {
				out.push_back(full_file_path);
			}

		} while (FindNextFile(dir, &file_data));

		FindClose(dir);
#else
		DIR* dir;
		class dirent* ent;
		class stat st;

		dir = opendir(directory);
		ent = readdir(dir);
		while (ent != NULL) {
			const string file_name = ent->d_name;
			const string full_file_path = directory + "/" + file_name;

			if (file_name[0] == '.') {
				continue;
			}

			if (stat(full_file_path.c_str(), &st) == -1) {
				continue;
			}

			const bool is_directory = (st.st_mode & S_IFDIR) != 0;

			if (is_directory) {
				if (recursive) {
					getFilesInDir(full_file_path, out, recursive, relative, base_dir);
				}
				else {
					continue;
				}
			}

			if (relative) {
				out.push_back(full_file_path.substr(base_dir.size()));
			}
			else {
				out.push_back(full_file_path);
			}
			ent = readdir(dir);
		}

		closedir(dir);
#endif
	}

	std::string stripExtension(const std::string& path) {
		return path.substr(0, path.find_last_of('.'));
	}

#ifdef _WIN32
	std::string getProcessPath() {
		std::string path;
		path.resize(_MAX_PATH);

		int len = GetModuleFileName(NULL, &path[0], static_cast<unsigned int>(path.size()));
		if (len <= path.size()) {
			path.resize(len);
			return path;
		}
		else {
			throw std::exception("Path too long!");
		}
	}
#else
#include <sys/stat.h>
#include <unistd.h>
	std::string getProcessPath() {
		std::string path;
		path.resize(_MAX_PATH);

		std::vector<std::string> links{ "/proc/self/exe", "/proc/curproc/file", "/proc/curproc/exe" };
		struct stat buffer;
		for (auto& path : links) {
			if (stat(path.c_str(), &buffer) == 0) {
				unsigned int readlink_result = readlink(path.c_str(), &path[0]);
				if (readlink_result < 0) {
					throw std::exception("Readlink err");
				}
				else if (readlink_result > path.size()) {
					throw std::exception("Path too long!");
				}
				else {
					return path;
				}
			}
		}

		throw std::exception("Process path could not be found!");
	}
#endif

	std::pair<std::string, std::string> splitIntoParentAndChildPath(const std::string& path) {
		auto dir_len = static_cast<unsigned int>(path.find_last_of(FILE_SEP_CHAR));
		return std::make_pair(path.substr(0, dir_len), path.substr(dir_len + 1));
	}
}