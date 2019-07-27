#pragma once

#include "job.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class GBufferJob : public Job
{
public:
	GBufferJob();
	~GBufferJob();
	void resetRTs();
	void run() override;

	void setMVP(const glm::mat4 &MVP) {	m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP); }

	void resize(int screenWidth, int screenHeight) override;

	eRenderPasses getJobType() override { return eRenderPasses::GBuffer; }

	GLuint getPositionRT() { return m_position; }
	GLuint getDepthRT() { return m_depth; }
	GLuint getAlbedoRT() { return m_albedo; }
	GLuint getNormalRT() { return m_normals; }
	GLuint getFramebuffer() { return m_gbuffer; }
private:
	
	GLuint m_gbuffer;
	GLuint m_position;
	GLuint m_depth;
	GLuint m_albedo;
	GLuint m_normals;
};

