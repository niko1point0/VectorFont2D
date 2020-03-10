#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

class Shader
{
	//OpenGL uses shader "programs"
	GLuint program, vprogram, fprogram; 

	//Default file names
	std::string  filenamev = "shaders/vshader.glsl";
	std::string  filenamef = "shaders/fshader.glsl";

public:
	Shader();
	Shader(std::string fnv, std::string fnf);
	~Shader();
	bool load();
	bool compile(GLenum shaderType);
	void printInfoLog(std::string filename, GLint index);
	void use();
	void unload();
};

