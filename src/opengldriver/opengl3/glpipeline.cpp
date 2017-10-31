#include "glpipeline.h"

GLPipeline::GLPipeline()
{
	glGenProgramPipelines(1, &m_pipeline);
}

void GLPipeline::attachShader(const std::string &path, GLuint shaderType)
{
	std::string contents = FileLoader::loadFile(path);
	const char *ptr = contents.c_str();

	auto program = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &ptr);
	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, program);
	m_programs.push_back(program);
}
