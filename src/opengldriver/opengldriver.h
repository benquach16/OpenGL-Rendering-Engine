#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define assert(ignore) ((void)0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"
#include "opengl/glprogram.h"
#include "opengl3/glpipeline.h"
#include "rendermanager.h"
#include "vertexbuffer.h"
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
	enum eRenderBuffers
	{
	};

	enum eRenderPipelines
	{
		Deferred,
		PostProcess,
		Framebuffer,
	};
	
	OpenGLDriver();
	~OpenGLDriver();

	void resize(ScreenInfo info);
	//generic render that renders everything
	void render();

	void initializeDriver();
	void loadShaderProgram();
	void run();
	void submit(VertexBuffer* buf);
protected:
	void renderScene();
	void renderQuad();
	void renderDeferred();
	std::vector<GLPipeline*> m_programPipelines;
	std::unordered_map<eRenderPipelines, GLPipeline*> m_renderPipelines;
	int m_currentPipeline;
	
	//temporary
	//should make a frame buffer object
	GLuint m_buffers[3];
	
	GLuint m_gbuffer;
	GLuint m_depth;
	GLuint m_albedo;
	GLuint m_normals;

	ScreenInfo m_screenInfo;

	RenderManager m_rendermanager;
};
