#include "opengldriver.h"
#include "../util/debug.h"

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
	//glEnable(GL_CULL_FACE);  
	
}

OpenGLDriver::~OpenGLDriver()
{
	for(auto i : m_programPipelines)
	{
		delete i;
	}
}

void OpenGLDriver::resize(ScreenInfo info)
{
	glGenFramebuffers(1, &m_gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, info.m_width, info.m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glGenTextures(1, &m_albedo);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.m_width, info.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &m_normals);
	glBindTexture(GL_TEXTURE_2D, m_normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.m_width, info.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedo, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normals, 0);	
	
	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers
	
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
	else
	{
		cerr << "framebuffer success" << endl;
	}
}


void OpenGLDriver::initializeDriver()
{
	glClearColor(0.0, 0.1, 0.6, 1.0);
	glewExperimental = GL_TRUE;
	glewInit();
	//refactor this
	loadShaderProgram();
}


//bootstrap code
void OpenGLDriver::loadShaderProgram()
{
	auto quad = new GLPipeline;
	quad->addShader("shaders/framebuffer.vert", GLProgram::SHADER_TYPE::VERTEX);
	quad->addShader("shaders/framebuffer.frag", GLProgram::SHADER_TYPE::FRAGMENT);
	m_programPipelines.push_back(quad);

	auto scene = new GLPipeline;
	scene->addShader("shaders/phong.vert", GLProgram::SHADER_TYPE::VERTEX);
	scene->addShader("shaders/phong.frag", GLProgram::SHADER_TYPE::FRAGMENT);
	m_programPipelines.push_back(scene);

	m_currentPipeline = 0;
}

void OpenGLDriver::submit(VertexBuffer* buf)
{
	//cerr << "pushing vertex buffer" << endl;
	m_rendermanager.push(buf);
}

void OpenGLDriver::render()
{
	renderScene();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0,0,800,600,
					  0,0,400,300,
					  GL_COLOR_BUFFER_BIT,
					  GL_NEAREST);
	GL_GET_ERROR("Error on BlitFramebuffer");
	//renderQuad(m_albedo, 0,0,400,300);
	//renderQuad(m_depth,400,0,800,300);
	//renderQuad();
}

void OpenGLDriver::renderScene()
{
	m_programPipelines[1]->bindPipeline();

	//mat = view * mat;
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.5f, 1.5f, 1.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Projection = glm::perspective(glm::radians(40.0f), 4.0f / 3.0f, 0.1f, 100.f);
	Projection = Projection * view;
	m_programPipelines[1]->setUniform(GLProgram::SHADER_TYPE::VERTEX, "MVP", Projection);
	glViewport(0, 0, 800, 600);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_rendermanager.render();
}

void OpenGLDriver::renderQuad()
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
	m_programPipelines[0]->bindPipeline();
	m_programPipelines[0]->setUniform(GLProgram::SHADER_TYPE::FRAGMENT, "depth", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depth);

	GL_GET_ERROR("Error rendering quad");
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void OpenGLDriver::renderQuad(GLuint buffer, int x0, int y0, int x1, int y1)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(x0, y0, x1, y1);
	m_programPipelines[0]->bindPipeline();
	m_programPipelines[0]->setUniform(GLProgram::SHADER_TYPE::FRAGMENT, "depth", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer);

	auto err = glGetError();
	if(err != GL_NO_ERROR)
	{
		cerr << "OpenGLDriver: render issue " << err << endl;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void OpenGLDriver::run()
{

		
}

