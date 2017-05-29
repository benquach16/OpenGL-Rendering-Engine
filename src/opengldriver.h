#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "util.h"
#include <fstream>


//screen info struct
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
	void loadShaders();
	void run();
	
protected:
	//temporary
	//should make a frame buffer object
	GLuint m_gbuffer;
	GLuint m_depth;
	GLuint m_albedo;

	//temporary
	//should make a class for shader programs
	GLuint m_vert;
	GLuint m_frag;

	ScreenInfo m_screenInfo;
};
