#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include "../../util/util.h"
#include "../opengl/glprogram.h"

class GLPipeline {
	friend class GLProgram;
public:
	GLPipeline();
	~GLPipeline();
	void addShader(std::string path, GLProgram::SHADER_TYPES type);
	void bindPipeline();
	void setUniform(GLProgram::SHADER_TYPES type, const std::string &uniform, int val);

	void create();
	void reset();
private:
	void addShader(const GLProgram *program);
	GLuint getShaderBit(GLProgram::SHADER_TYPES type);
	GLuint m_pipeline;
	//std::vector<GLProgram> m_programs;
	//	std::unordered_map<GLProgram::SHADER_TYPES, shared_ptr<GLProgram> > m_programs;
	std::unordered_map<GLProgram::SHADER_TYPES, GLProgram*> m_programs;
};
