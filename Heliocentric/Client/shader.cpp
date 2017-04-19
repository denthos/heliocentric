

#include <iostream>

#include "shader.h"

GLuint Shader::currentlyBoundShaderID = 0x0;

Shader::Shader(const char *vert, const char *frag)
{
	
    //Read in the vertex and fragment shaders
    //We must delete these after we are finished compiling the shaders
	std::string vv = read(vert);
	std::string vf = read(frag);
        
    //Setup the shader
	setup(vv, vf);


}

Shader::~Shader()
{

	glDeleteProgram(pid);
}

void Shader::bind()
{
    if(currentlyBoundShaderID != pid)
    {
        currentlyBoundShaderID = pid;
        glUseProgram(pid);
    }
}

void Shader::unbind()
{
    if(currentlyBoundShaderID != (0x0))
    {
        currentlyBoundShaderID = (0x0);
        glUseProgram(0);
    }
}

GLhandleARB Shader::getPid()
{
	return pid;
}



std::string Shader::read(const char *filename)
{
	std::ifstream in(filename);
	std::stringstream shaderSource;
	shaderSource << in.rdbuf();
	
	
    
	return shaderSource.str();
}

bool Shader::setup(std::string vertSource, std::string fragSource)
{
	/* Based on https://www.khronos.org/opengl/wiki/Example/GLSL_Full_Compile_Linking */


	//Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Send the vertex shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	const GLchar *source = (const GLchar *)vertSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	//Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(vertexShader);

		//Dump the infolog
		fprintf(stderr, "%s\n", &infoLog[0]);

		//Give error message and return
		fprintf(stderr, "Failed to compile vertex shader.\n");
		return false;
	}

	//Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Send the fragment shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar *)fragSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	//Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		//Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		//Dump the infolog
		fprintf(stderr, "%s\n", &infoLog[0]);

		//Give error message and return
		fprintf(stderr, "Failed to compile fragment shader.\n");
		return false;
	}

	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	pid = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(pid, vertexShader);
	glAttachShader(pid, fragmentShader);

	//Link our program
	glLinkProgram(pid);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(pid, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(pid, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(pid);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//Dump the infolog
		fprintf(stderr, "%s\n", &infoLog[0]);

		//Give error message and return
		fprintf(stderr, "Failed to link shaders.\n");
		return false;
	}

	//Always detach shaders after a successful link.
	glDetachShader(pid, vertexShader);
	glDetachShader(pid, fragmentShader);


	return true;
}



