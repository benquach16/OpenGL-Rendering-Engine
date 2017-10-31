#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include "../../util/util.h"

class GLPipeline {
public:
	enum SHADER_TYPES {
		VERTEX,
		GEOMETRY,
		FRAGMENT
	};
	GLPipeline();
	void attachShader(const std::string &path, GLuint shaderType);
private:
	GLuint m_pipeline;
	std::vector<GLuint> m_programs;
};
