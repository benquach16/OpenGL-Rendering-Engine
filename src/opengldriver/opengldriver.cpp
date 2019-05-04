#include "opengldriver.h"
#include "../util/debug.h"

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 
//0: vertex pos
//1: texcoord
GLfloat quad[] = {
    //upper left triangle
	-1.0f, -1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	0.0f, 1.0f,

	//bottom right triangle
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 0.0f
};

GLfloat box[] = {
	// front face
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,

	// back face
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};

GLfloat indices[] = {0, 1, 2};



OpenGLDriver::OpenGLDriver()
{	
}

OpenGLDriver::~OpenGLDriver()
{
	for(auto i : m_renderPipelines)
	{
		delete i.second;
	}
	m_renderPipelines.clear();
}

void OpenGLDriver::resize(ScreenInfo info)
{
	m_renderManager.resize(info.m_width, info.m_height);
	m_screenInfo = info;
}


void OpenGLDriver::initializeDriver()
{
	glClearColor(0.0, 0.1, 0.6, 1.0);
	glewExperimental = GL_TRUE;
	glewInit();
	m_renderManager.initRenderPipelines();
}


void OpenGLDriver::submit(VertexBuffer* buf)
{
	//cerr << "pushing vertex buffer" << endl;
	m_renderManager.push(buf, eRenderPasses::GBuffer);
}

void OpenGLDriver::setCameraPerspective(const glm::mat4 &MVP, const glm::vec3 &cameraPosition)
{
	//ASSERT(m_renderPipelines[eRenderPipelines::Deferred] != nullptr, "No Deferred rendering pipeline created");
	//m_renderPipelines[eRenderPipelines::Deferred]->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP);
	m_renderManager.setCameraPerspective(MVP);
}

void OpenGLDriver::render()
{
	renderScene();
	//renderQuad();
}

void OpenGLDriver::renderScene()
{
	glViewport(0, 0, m_screenInfo.m_width, m_screenInfo.m_height);

	m_renderManager.render();
}

void OpenGLDriver::renderLightVolumes()
{
	// todo : defer this so we dont alloc memory every frame
	GLuint vertarray;
	glGenVertexArrays(1, &vertarray);
	glBindVertexArray(vertarray);
	GLuint vertices;
	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, BUFFER_OFFSET(12));
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &vertices);
	glDeleteBuffers(1, &vertarray);
}
