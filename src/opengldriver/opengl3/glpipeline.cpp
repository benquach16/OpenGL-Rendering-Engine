#include <iostream>
#include "glpipeline.h"

using namespace std;

GLPipeline::GLPipeline()
{
	glGenProgramPipelines(1, &m_pipeline);
}

GLPipeline::~GLPipeline()
{
	for(auto i : m_programs)
	{
		delete i.second;
	}
}

void GLPipeline::addShader(const GLProgram *program)
{
	glUseProgramStages(m_pipeline, getShaderBit(program->m_shaderType), program->m_program);
	auto err = glGetError();
	if(err != GL_NO_ERROR)
	{
		cerr << "error using shader pipeline: " << err << " on program " << program->m_path << endl;
	}
	//m_programs.push_back(program);
}

void GLPipeline::addShader(std::string path, GLProgram::SHADER_TYPES type)
{
	auto program = new GLProgram(path, type);
	m_programs[type] = program;
	addShader(program);
}

void GLPipeline::bindPipeline()
{
	glBindProgramPipeline(m_pipeline);
}

void GLPipeline::setUniform(GLProgram::SHADER_TYPES type, const std::string &uniform, int val)
{
	if(m_programs[type])
	{
		m_programs[type]->setUniform(uniform, val);
	}

}

GLuint GLPipeline::getShaderBit(GLProgram::SHADER_TYPES type)
{ 
	switch (type)
	{
	case GLProgram::SHADER_TYPES::VERTEX:
		return GL_VERTEX_SHADER_BIT;
	case GLProgram::SHADER_TYPES::GEOMETRY:
		return GL_GEOMETRY_SHADER_BIT;
	case GLProgram::SHADER_TYPES::FRAGMENT:
		return GL_FRAGMENT_SHADER_BIT;
	}
	return -1;
}
