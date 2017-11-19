#include "glpipeline.h"

GLPipeline::GLPipeline()
{
	glGenProgramPipelines(1, &m_pipeline);
}

void GLPipeline::addShader(const GLProgram &program)
{
	std::string contents = FileLoader::loadFile(path);
	const char *ptr = contents.c_str();

	glUseProgramStages(m_pipeline, getShaderBit(program.m_shaderType), program.m_program);
	m_programs.push_back(program);
}

void GLPipeline::addShader(std::string path, GLProgram::SHADER_TYPES type)
{
	std::string contents = FileLoader::loadFile(path);
	const char *ptr = contents.c_str();
   
	glUseProgramStages(m_pipeline, getShaderBit(type), 
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
