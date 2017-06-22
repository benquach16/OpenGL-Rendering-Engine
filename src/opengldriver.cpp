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
}

void OpenGLDriver::loadShaders()
{
	m_vert = glCreateShader(GL_VERTEX_SHADER);
	m_frag = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vert = FileLoader::loadFile("shaders/vs_general.glsl");
	std::string frag = FileLoader::loadFile("shaders/fs_general.glsl");

	const char *ptr = vert.c_str();
	glShaderSource(m_vert, 1, &ptr, 0);
	glCompileShader(m_vert);


	int err = 0;
	GLint result = GL_FALSE;

	glGetShaderiv(m_vert, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_vert, GL_INFO_LOG_LENGTH, &err);
	if(err > 0)
	{
		cerr << "error with compiling vertex shader" << endl;
	}

	ptr = frag.c_str();
	glShaderSource(m_frag, 1, &ptr, 0);
	glCompileShader(m_frag);

	glGetShaderiv(m_frag, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_frag, GL_INFO_LOG_LENGTH, &err);

	if(err > 0)
	{
		cerr << "error with compiling pixel shader" << endl;
	}

	
	
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

