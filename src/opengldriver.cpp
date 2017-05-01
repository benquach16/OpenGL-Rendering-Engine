#include "opengldriver.h"


OpenGLDriver::OpenGLDriver()
{

	
}

OpenGLDriver::~OpenGLDriver()
{
}

void OpenGLDriver::initializeDriver()
{
	glGenFramebuffers(1, &m_albedo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_albedo);
	glClearColor(0.1,0.1,0.7,1.0);
	glClear( GL_COLOR_BUFFER_BIT );
	glClearDepth(1.0f);	
}
