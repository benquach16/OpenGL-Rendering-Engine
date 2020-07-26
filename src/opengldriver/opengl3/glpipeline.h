#pragma once

#include <GL/glew.h>

#include "../../util/util.h"
#include "../opengl/glprogram.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

class GLPipeline {
    friend class GLProgram;

public:
    GLPipeline();
    ~GLPipeline();
    void addShader(std::string path, GLProgram::eShaderType type);
    void bindPipeline();
    void setUniform(GLProgram::eShaderType type, const std::string& uniform, int val);
    void setUniform(GLProgram::eShaderType type, const std::string& uniform, glm::mat4 val);
    void setUniform(GLProgram::eShaderType type, const std::string& uniform, glm::vec3 val);
    void setUniform(GLProgram::eShaderType type, const std::string& uniform, glm::vec2 val);
    void create();
    void reset();

private:
    void addShader(const GLProgram* program);
    GLuint getShaderBit(GLProgram::eShaderType type);
    GLuint m_pipeline;
    //std::vector<GLProgram> m_programs;
    //	std::unordered_map<GLProgram::SHADER_TYPE, shared_ptr<GLProgram> > m_programs;
    std::unordered_map<GLProgram::eShaderType, GLProgram*> m_programs;
};
