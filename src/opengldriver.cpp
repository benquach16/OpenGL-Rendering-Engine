#include "opengldriver.h"


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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depth, 0);

		
}

void OpenGLDriver::initializeDriver()
{
}

void OpenGLDriver::run()
{
	//render scene here
	//temp triangle
	GLfloat verts[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};  
	GLuint vertices;
	glGenBuffers(1, &vertices);
	glBindBuffers(GL_ARRAY_BUFFERS, &vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
}

