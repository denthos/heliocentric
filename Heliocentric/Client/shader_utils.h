#pragma once

#include <GL\glew.h>
#include <string>

bool compileShaders(const char * vertexFile, const char * fragmentFile, GLuint *);

bool compileShadersWithSource(std::string vertexSource, std::string fragmentSource, GLuint *);