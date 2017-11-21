#include "glprogram.h"

using namespace std;

GLProgram::GLProgram(std::string path, SHADER_TYPES shaderType) : m_program(0), m_shaderType(shaderType), m_path(path)
{
	create();
}

GLProgram& GLProgram::operator=(const GLProgram &rhs)
{
	reset();
	if(rhs.m_program)
	{
		this->m_program = rhs.m_program;
	}
	this->m_uniforms = rhs.m_uniforms;
	this->m_shaderType = rhs.m_shaderType;
	return *this;
}

GLProgram::~GLProgram()
{
	reset();
}

void GLProgram::create()
{
	reset();
	std::string contents = FileLoader::loadFile(m_path);
	const char *ptr = contents.c_str();

	auto id = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &ptr);
	if(!id)
	{
		//error!
		
	}
	m_program = id;
}

void GLProgram::reset()
{
	if(m_program)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}
