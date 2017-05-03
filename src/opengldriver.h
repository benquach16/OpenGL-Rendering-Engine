#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


struct ScreenInfo
{
	int m_width;
	int m_height;
ScreenInfo() : m_width(2), m_height(2){}
};

class OpenGLDriver
{
public:
	OpenGLDriver();
	~OpenGLDriver();

	void resize(ScreenInfo info);
	void initializeDriver();
	void initializeBuffers();
	void run();
	
protected:
	//temporary
	//should make a frame buffer object
	GLuint m_gbuffer;
	GLuint m_depth;
	GLuint m_albedo;

	ScreenInfo m_screenInfo;
};
