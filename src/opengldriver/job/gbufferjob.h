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

	void initRTs(int width, int height);
	void run() override;

	void setMVP(const glm::mat4 &MVP);
private:	
	GLuint m_gbuffer;
	GLuint m_position;
	GLuint m_depth;
	GLuint m_albedo;
	GLuint m_normals;
};

