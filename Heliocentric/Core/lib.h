/**
@file lib.h
@brief A library containing all general utility methods.
*/
#pragma once
#include <iostream>
#include <string>

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

	/**
	Debug function that prints the message sent as a parameter to std::cerr.
	@param message The message that is to be printed.
	*/
	void debug(std::string message);
};