#include "opengldriver.h"

using namespace std;

OpenGLDriver::OpenGLDriver()
{

	
}

OpenGLDriver::~OpenGLDriver()
{
}

void OpenGLDriver::resize(ScreenInfo info)
{

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenInfo.m_width, m_screenInfo.m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depth, 0);

		
}


void OpenGLDriver::initializeDriver()
{
	glClearColor(0.0, 0.1, 0.6, 0.0);
	glewExperimental = GL_TRUE;
	glewInit();
	loadShaders();
	//refactor this
	loadShaderProgram();
}


void OpenGLDriver::loadShaders()
{
	m_vert = loadShader("shaders/vs_general.glsl", GL_VERTEX_SHADER);
	m_frag = loadShader("shaders/fs_general.glsl", GL_FRAGMENT_SHADER);
}

int OpenGLDriver::loadShader(std::string path, GLuint shaderType)
{
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
		cerr << "error with comping shader " + path << endl;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logsize);
		//since we don't want to new anything
		string errorLog(logsize, ' ');
		glGetShaderInfoLog(id, logsize, &logsize, &errorLog[0]);
		cerr << errorLog.c_str() << endl;
													   
	}
	return id;
}

void OpenGLDriver::loadShaderProgram()
{
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vert);
	glAttachShader(m_program, m_frag);
	glLinkProgram(m_program);

	GLuint result = GL_FALSE;
	glUseProgram(m_program);
	
}

void OpenGLDriver::run()
{

	GLuint vertarray;
	glGenVertexArrays(1, &vertarray);
	glBindVertexArray(vertarray);
	//render scene here
	//temp triangle
	GLfloat verts[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};  
	GLuint vertices;
	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	

	
}

