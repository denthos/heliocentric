/**
@file lib.h
@brief A library containing all general utility methods.
*/
#pragma once
#include <glm\glm.hpp>
#include <iostream>
#include <string>

/**
A "static" class that should never be instantiated. All methods within this
class should be public and static.
*/
class Lib {
public:
	/**
	Prints out error message if Expression passed in evaluates to false.
	@parem expression Expression to be evalueted.
	@param message Message to be printed out if assertion fails.
	*/
	static void assertTrue(bool expression, std::string message);

	/**
	Debug function that prints the message sent as a parameter to std::cerr.
	@param message The message that is to be printed.
	*/
	static void debug(std::string message);
	
	/**
	Calculates the distance between two locations.
	@param position1 The first location.
	@param position2 The second location.
	@return The distance between position1 and position2.
	*/
	static float distance_vec3(glm::vec3 position1, glm::vec3 position2);
};