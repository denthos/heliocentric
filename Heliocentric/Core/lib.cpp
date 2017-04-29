#include "lib.h"
#include "logging.h"

namespace Lib {
	void assertTrue(bool expression, std::string message) {
		if (!expression) {
			LOG_ERR(message);
		}
	}
}