/**
@file lib.h
@brief A library containing all general utility methods.
*/
#pragma once
#include <iostream>
#include <string>

#include "logging.h"

/**
A "static" class that should never be instantiated. All methods within this
class should be public and static.
*/
namespace Lib {
	/**
	Prints out error message if Expression passed in evaluates to false.
	@param expression Expression to be evaluated.
	@param message Message to be printed out if assertion fails.
	*/
	void assertTrue(bool expression, std::string message);

	template <class T, class U>
	bool assertNotEqual(const T& unexpected, const U& actual, std::string message) {
		if (unexpected == actual) {
			LOG_ERR(message);
			return false;
		}

		return true;
	}
	bool floatCompare(float a, float b, float threshold = 0.5);

};