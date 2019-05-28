#include "directlightingjob.h"
#include "gbufferjob.h"
#include "../../util/debug.h"

#include <iostream>

using namespace std;

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

DirectLightingJob::DirectLightingJob() : m_framebuffer(0), m_albedo(0)
{
	setVertexShader("shaders/framebuffer.vert");
	setFragmentShader("shaders/directlighting.frag");
}

void DirectLightingJob::run()
{	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// make sure we have a properly initialized job
	ASSERT(m_width > 0, "Screen Width not set for Direct Lighting Pass");
	ASSERT(m_height > 0, "Screen Height not set for Direct Lighting Pass");
	
	glViewport(0, 0, m_width, m_height);
	m_pipeline->bindPipeline();
	m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDepth", 0);
	m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uAlbedo", 1);
	m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uNormals", 2);

	// make sure that our parent is a gbuffer job (DAG strictly enforced)
	ASSERT(m_parent != nullptr, "DAG initialized incorrectly");
	ASSERT(m_parent->getJobType() == eRenderPasses::GBuffer, "Parent job of incorrect type");

	GBufferJob* parent = static_cast<GBufferJob*>(m_parent);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, parent->getPositionRT());
	GET_GL_ERROR("Error setting position render target");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, parent->getAlbedoRT());
	GET_GL_ERROR("Error setting albedo render target");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, parent->getNormalRT());
	GET_GL_ERROR("Error setting normal render target");
	
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

void DirectLightingJob::resize(int width, int height)
{
	Job::resize(width, height);
	
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glGenTextures(1, &m_albedo);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedo, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

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

