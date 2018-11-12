#include <iostream>
#include "glpipeline.h"
#include "../util/debug.h"

using namespace std;

GLPipeline::GLPipeline()
{
	create();
}

GLPipeline::~GLPipeline()
{
	//important - delete all programs
	reset();
}

void GLPipeline::create()
{
	glGenProgramPipelines(1, &m_pipeline);
}

void GLPipeline::reset()
{
	glDeleteProgramPipelines(1, &m_pipeline);
	for(auto i : m_programs)
	{
		delete i.second;
	}
}

void GLPipeline::addShader(const GLProgram *program)
{
	glUseProgramStages(m_pipeline, getShaderBit(program->m_shaderType), program->m_program);
	auto err = glGetError();
	while(err != GL_NO_ERROR)
	{
		cerr << "Error adding shader to program pipeline: " << err << " on shader program " << program->m_path << endl;
		err = glGetError();	
	}
}

void GLPipeline::addShader(std::string path, GLProgram::eShaderType type)
{
	//if we find a shader type thats already been added
	//we could also overwrite
	ASSERT(m_programs.find(type) == m_programs.end(), "shader program type already found in pipeline");
	auto program = new GLProgram(path, type);
	m_programs[type] = program;
	addShader(program);
}

void GLPipeline::bindPipeline()
{
	glBindProgramPipeline(m_pipeline);
}

void GLPipeline::setUniform(GLProgram::eShaderType type, const std::string &uniform, int val)
{
	ASSERT(m_programs.find(type) != m_programs.end(), "shader program for this type has not yet been set");
	m_programs[type]->setUniform(uniform, val);
}

void GLPipeline::setUniform(GLProgram::eShaderType type, const std::string &uniform, glm::mat4 val)
{
	ASSERT(m_programs.find(type) != m_programs.end(), "shader program for this type has not yet been set");
	m_programs[type]->setUniform(uniform, val);
}

void GLPipeline::setUniform(GLProgram::eShaderType type, const std::string &uniform, glm::vec3 val)
{
	ASSERT(m_programs.find(type) != m_programs.end(), "shader program for this type has not yet been set");
	m_programs[type]->setUniform(uniform, val);
}

GLuint GLPipeline::getShaderBit(GLProgram::eShaderType type)
{ 
	switch (type)
	{
	case GLProgram::eShaderType::Vertex:
		return GL_VERTEX_SHADER_BIT;
	case GLProgram::eShaderType::Geometry:
		return GL_GEOMETRY_SHADER_BIT;
	case GLProgram::eShaderType::Fragment:
		return GL_FRAGMENT_SHADER_BIT;
	}
	return -1;
}
