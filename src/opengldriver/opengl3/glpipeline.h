#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include "../../util/util.h"
#include "../opengl/glprogram.h"

class GLPipeline {
	friend class GLProgram;
public:
	GLPipeline();
	void addShader(const GLProgram &program);
	void addShader(std::string path, GLProgram::SHADER_TYPES type);
	void bindPipeline();
private:
	GLuint getShaderBit(GLProgram::SHADER_TYPES type);
	GLuint m_pipeline;
	std::vector<GLProgram> m_programs;
};
