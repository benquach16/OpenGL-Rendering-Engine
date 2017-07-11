#include "glprogram.h"

using namespace std;

GLProgram::GLProgram() : m_program(0)
{
	create();
}

GLProgram::~GLProgram()
{
	reset();
}

void GLProgram::create()
{
	reset();
	m_program = glCreateProgram();
	glLinkProgram(m_program);
}

void GLProgram::reset()
{
	if(m_program)
	{
		glDeleteProgram(m_program);
		for(auto i : m_shaderIds)
		{
			//delete shader instances here
			glDeleteShader(i);
		}
		m_program = 0;
	}
}

int GLProgram::attachShader(std::string path, SHADER shaderType)
{
	if(!m_program)
	{
		cerr << "shader program not initialized when trying to create shader " << path << endl;
	}
	int id = glCreateShader(shaderType);
	std::string contents = FileLoader::loadFile(path);
	const char *ptr = contents.c_str();
	glShaderSource(id, 1, &ptr, 0);
	glCompileShader(id);

	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		//error!
		int error = 0;
		int logsize = 0;
		cerr << "error with compiling shader " + path << endl;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logsize);
		string errorLog(logsize, '\0');
		glGetShaderInfoLog(id, logsize, &logsize, &errorLog[0]);
		cerr << errorLog << endl;
													   
	}
	m_shaderIds.push_back(id);
	
	return id;
}

void GLProgram::activateProgram()
{
	glUseProgram(m_program);
}
