#pragma once

#include <string>
#include "util.h"

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
	void clearAll();
	void attachShader(std::string path, SHADER shaderType);
protected:
	
		
};
