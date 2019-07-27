#include "opengldriver.h"
#include "../util/debug.h"

using namespace std;

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

void OpenGLDriver::setCameraPerspective(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPosition)
{
	//ASSERT(m_renderPipelines[eRenderPipelines::Deferred] != nullptr, "No Deferred rendering pipeline created");
	//m_renderPipelines[eRenderPipelines::Deferred]->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP);
	m_renderManager.setCameraPerspective(view, projection);
}

void OpenGLDriver::render()
{
	renderScene();
}

void OpenGLDriver::renderScene()
{
	glViewport(0, 0, m_screenInfo.m_width, m_screenInfo.m_height);

	m_renderManager.render();
}

void OpenGLDriver::renderLightVolumes()
{
	// todo : defer this so we dont alloc memory every frame

}
