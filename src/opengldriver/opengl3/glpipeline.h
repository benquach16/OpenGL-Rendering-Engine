#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
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
	void addShader(std::string path, GLProgram::SHADER_TYPE type);
	void bindPipeline();
	void setUniform(GLProgram::SHADER_TYPE type, const std::string &uniform, int val);
	void setUniform(GLProgram::SHADER_TYPE type, const std::string &uniform, Mat4 mat);
	void setUniform(GLProgram::SHADER_TYPE type, const std::string &uniform, glm::mat4 val);	
	void create();
	void reset();
private:
	void addShader(const GLProgram *program);
	GLuint getShaderBit(GLProgram::SHADER_TYPE type);
	GLuint m_pipeline;
	//std::vector<GLProgram> m_programs;
	//	std::unordered_map<GLProgram::SHADER_TYPE, shared_ptr<GLProgram> > m_programs;
	std::unordered_map<GLProgram::SHADER_TYPE, GLProgram*> m_programs;
};
