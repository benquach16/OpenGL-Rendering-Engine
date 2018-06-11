#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../util/util.h"
#include "../../renderer/mat4.h"

//this class is a wrapper around the opengl program reference, it should
//be able to be copied and point to the same program
class GLProgram
{
	friend class GLPipeline;
public:
	enum SHADER_TYPE
	{
		VERTEX,
		TESSELATION,
		GEOMETRY,
		FRAGMENT
	};

	enum ATTRIBUTE_UNIFORM_TYPE
	{
		INT,
		FLOAT,
		MAT3,
		MAT4,
		VEC2,
		VEC3,
		VEC4,

		//Uniform exclusive
		SAMPLER1D,
		SAMPLER2D,
		SAMPLER3D
	};
	
	GLProgram(std::string path, SHADER_TYPE shaderType);
	GLProgram& operator=(const GLProgram &rhs);
	~GLProgram();

	void create();
	void reset();
	void setUniform(const std::string &uniform, int val);
	void setUniform(const std::string &uniform, Mat4 val);
	void setUniform(const std::string &uniform, glm::mat4 &val);
	SHADER_TYPE getProgramType();
protected:
	static GLuint         getShaderBit(SHADER_TYPE type);
	static ATTRIBUTE_UNIFORM_TYPE getAttributeFromGL(GLint type);
	void                  getShaderInputs();
	
	//reimplementation of glCreateShaderProgramv
	int                                  createShaderProgram(GLenum type, const char **str);
	std::vector<GLuint>                  m_shaderIds;
	GLuint                               m_program;
	SHADER_TYPE                          m_shaderType;
	std::string                          m_path;
	std::unordered_map<std::string, ATTRIBUTE_UNIFORM_TYPE> m_attributes;
	std::unordered_map<std::string, ATTRIBUTE_UNIFORM_TYPE> m_uniforms;
};
