#ifndef SHADERUTILS_H_
#define SHADERUTILS_H_

#include <GL\glew.h>
#include <string>

bool compileShaders(const char * vertexFile, const char * fragmentFile, GLuint *);

bool compileShadersWithSource(std::string vertexSource, std::string fragmentSource, GLuint *);

#endif