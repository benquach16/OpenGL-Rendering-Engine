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
	auto id = glCreateShaderProgramv(getShaderBit(m_shaderType), 1, &ptr);
	if(!id)
	{
		//error!
		cerr << "error creating shaderprogram" << endl;
	}
	auto err = glGetError();
	if(err != GL_NO_ERROR)
	{
		cerr << "error creating shaderprogram " << err << endl;
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

void GLProgram::setUniform(const std::string &uniform, int val)
{
	if(!m_program)
	{
		cerr << "attempted to set uniform on uninitialized program" << endl;
		return;
	}
	m_uniforms[uniform] = val;
	auto location = glGetUniformLocation(m_program, uniform.c_str());
	auto err = glGetError();
	if(err != GL_NO_ERROR)
	{
		cerr << "GLProgram: uniform error " << err << endl;
	}
	glProgramUniform1i(m_program, location, val);
}

GLuint GLProgram::getShaderBit(GLProgram::SHADER_TYPES type)
{ 
	switch (type)
	{
	case GLProgram::SHADER_TYPES::VERTEX:
		return GL_VERTEX_SHADER;
	case GLProgram::SHADER_TYPES::GEOMETRY:
		return GL_GEOMETRY_SHADER;
	case GLProgram::SHADER_TYPES::FRAGMENT:
		return GL_FRAGMENT_SHADER;
	}
	return -1;
}
