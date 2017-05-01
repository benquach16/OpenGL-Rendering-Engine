#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


class OpenGLDriver
{
public:
	OpenGLDriver();
	~OpenGLDriver();
	void initializeDriver();
	
protected:
	//temporary
	//should make a frame buffer object
	GLuint m_depthbuffer;
	GLuint m_albedo;
};
