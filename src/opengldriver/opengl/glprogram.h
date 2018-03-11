#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../../util/util.h"

//this class is a wrapper around the opengl program reference, it should
//be able to be copied and point to the same program
class GLProgram
{
	friend class GLPipeline;
public:
	enum SHADER_TYPES
	{
		VERTEX,
		TESSELATION,
		GEOMETRY,
		FRAGMENT
	};

	enum ATTRIBUTE_TYPE
	{
		INT,
		FLOAT,
		MAT3,
		MAT4,
		VEC2,
		VEC3,
		VEC4
	};

	enum UNIFORM_TYPE
	{
		TEX
	};
	
	GLProgram(std::string path, SHADER_TYPES shaderType);
	GLProgram& operator=(const GLProgram &rhs);
	~GLProgram();

	void create();
	void reset();
	void setUniform(const std::string &uniform, int val);
	SHADER_TYPES getProgramType();
protected:
	static GLuint getShaderBit(SHADER_TYPES type);
	void getShaderInputs();
	
	//reimplementation of glCreateShaderProgramv
	int createShaderProgram(GLenum type, const char **str);
	std::vector<GLuint> m_shaderIds;
	GLuint m_program;
	SHADER_TYPES m_shaderType;
	std::string m_path;
	std::unordered_map<std::string, int> m_uniforms;
	std::vector<std::string> m_attributes;
};
