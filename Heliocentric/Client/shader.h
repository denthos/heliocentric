
#pragma once

#include <glad\glad.h>

#include <string>

/*! Handles GLSL shaders.  It can load the code from a file or read straight
 * from a char array. */
class Shader
{
	
private:
	

	std::string read(const char* filename); //read shader file

	bool setup(std::string vertSource, std::string fragSource, std::string geoSource); //compile shaders

protected:
	
	GLuint pid; //program id of shader
	static GLuint currentlyBoundShaderID;
	
public:
	
	Shader(){}
	Shader(const char* vert, const char* frag, const char* geo  = NULL); //constructor : takes in file names of shaders
	~Shader(); //destructor
	
	void bind() const; //when you want to use shader program
	void unbind() const; //stop using shader
	
	GLuint getPid() const;
	
	
	
};


