#include "lib.h"
#include "logging.h"

namespace Lib {
	void assertTrue(bool expression, std::string message) {
		if (!expression) {
			LOG_ERR(message);
		}
	}

	bool floatCompare(float a, float b, float threshold) {
		if (std::abs(a - b) <= threshold)
			return true;
		return false;
	}

}