#include "glpipeline.h"

GLPipeline::GLPipeline()
{
	glGenProgramPipelines(1, &m_pipeline);
}

void GLPipeline::attachProgram(GLProgram program)
{
	m_programs.push_back(program);
	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, program);
}
