#include "opengldriver.h"

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 
//0: vertex pos
//1: texcoord
GLfloat quad[] = {
    //upper left vert
	-1.0f, -1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	0.0f, 1.0f,

	//bottom right vert
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 0.0f
};



OpenGLDriver::OpenGLDriver()
{

	
}

OpenGLDriver::~OpenGLDriver()
{

}

void OpenGLDriver::resize(ScreenInfo info)
{

	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.m_width, info.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glGenTextures(1, &m_albedo);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.m_width, info.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	glGenFramebuffers(1, &m_gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depth, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		{
			cerr << "GL_FRAMEBUFFER: incomplete attachment error" << endl;
		}
		if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		{
			cerr << "GL_FRAMEBUFFER: incomplete missing attachment" << endl;
		}
		cerr << "framebuffer error" << endl;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void OpenGLDriver::initializeDriver()
{
	glClearColor(0.0, 0.1, 0.6, 0.0);
	glewExperimental = GL_TRUE;
	glewInit();
	//refactor this
	loadShaderProgram();
}


//bootstrap code
void OpenGLDriver::loadShaderProgram()
{
	GLPipeline pipeline;
	
	pipeline.addShader("shaders/framebuffer.vert", GLProgram::SHADER_TYPES::VERTEX);
	pipeline.addShader("shaders/framebuffer.frag", GLProgram::SHADER_TYPES::FRAGMENT);
	m_programPipelines.push_back(pipeline);
	pipeline.setUniform(GLProgram::SHADER_TYPES::FRAGMENT, "depth", m_depth);
	pipeline.bindPipeline();
	m_currentPipeline = 0;
}

void OpenGLDriver::render()
{
}

void OpenGLDriver::renderScene()
{
	
}

void OpenGLDriver::renderQuad()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_albedo);

	
	//m_programs[m_currentProgram]->activateProgram();
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
	
}

void OpenGLDriver::run()
{

		
}

