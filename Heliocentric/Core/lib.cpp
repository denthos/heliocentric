#include "lib.h"
namespace Lib {
	void assertTrue(bool expression, std::string message) {
		if (!expression) {
			std::cerr << message << std::endl;
		}
	}

	void debug(std::string message) {
		std::cerr << message << std::endl;
	}
}