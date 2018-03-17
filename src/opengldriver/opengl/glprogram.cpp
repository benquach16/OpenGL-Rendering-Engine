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
	//auto id = glCreateShaderProgramv(getShaderBit(m_shaderType), 1, &ptr);
	auto id = createShaderProgram(getShaderBit(m_shaderType),&ptr);

	if(!id)
	{
		//error!
		cerr << "error creating shaderprogram" << endl;
	}
	
	auto err = glGetError();
	while(err != GL_NO_ERROR)
	{
		cerr << "error creating shaderprogram " << err << " with shader " << m_path << endl;
		err = glGetError();
	}
	m_program = id;
	getShaderInputs();
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
	//m_uniforms[uniform] = val;
	auto location = glGetUniformLocation(m_program, uniform.c_str());
	auto err = glGetError();
	if(err != GL_NO_ERROR)
	{
		cerr << "GLProgram: uniform error " << err << endl;
	}
	glProgramUniform1i(m_program, location, val);
}


//create our own glCreateShaderProgram function because we want our own logging
int GLProgram::createShaderProgram(GLenum type, const char **str)
{
	const GLuint shader = glCreateShader(type);
	if (shader)
	{
		glShaderSource(shader, 1, (const GLchar**)str, NULL);
		glCompileShader(shader);
		const GLuint program = glCreateProgram();
		if (program) {
			GLint compiled = GL_FALSE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
			if (compiled)
			{
				glAttachShader(program, shader);
				glLinkProgram(program);
				glDetachShader(program, shader);
			}
			else
			{
				cerr << "Error: shader compilation for " << m_path << endl;
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
				for(auto i : errorLog)
				{
					cerr << i;
				}
			}
			/* append-shader-info-log-to-program-info-log */
		}
		glDeleteShader(shader);
		return program;
	} else {
		return 0;
	}
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

void GLProgram::getShaderInputs()
{

	//get shader inputs
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(m_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);


	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	std::vector<GLint> values(properties.size());
	
	for(int i = 0; i < numActiveAttribs; ++i)
	{
		glGetProgramResourceiv(m_program, GL_PROGRAM_INPUT, i, properties.size(),
							   &properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]); //The length of the name.
		glGetProgramResourceName(m_program, GL_PROGRAM_INPUT, i, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);
		cout << m_path << " found input " << name << endl;
		
	}

	for(int i = 0; i < numActiveUniforms; ++i)
	{
		glGetProgramResourceiv(m_program, GL_UNIFORM, i, properties.size(),
							   &properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]); //The length of the name.
		glGetProgramResourceName(m_program, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);
		cout << m_path << " found uniform " << name << endl;		
	}
}


