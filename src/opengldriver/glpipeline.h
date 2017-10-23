#pragma once

#include <GL/gl.h>
#include "glprogram.h"

class GLPipeline {
public:
	GLPipeline();

	void attachProgram(GLProgram program);

private:
	GLuint m_pipeline;
	std::vector<GLProgram> m_programs;
};
