#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "util.h"

//please don't expose gl things

//this class is a wrapper around the opengl program reference, it should
//be able to be copied and point to the same program
class GLProgram
{
public:
	enum SHADER
	{
		VERTEX,
		FRAGMENT
	};
	GLProgram();
	~GLProgram();

	void create();
	void reset();
	int attachShader(std::string path, SHADER shaderType);
	//beware of hotswapping programs, can have low performance
	//try ubershaders?
	void activateProgram();
protected:
	std::vector<int> m_shaderIds;
	int m_program;
	
};
