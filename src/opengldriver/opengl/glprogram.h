#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../renderer/mat4.h"
#include "../../util/util.h"

//this class is a wrapper around the opengl program reference, it should
//be able to be copied and point to the same program
class GLProgram {
    friend class GLPipeline;

public:
    enum eShaderType {
        Vertex,
        Tesselation,
        Geometry,
        Fragment,
    };

    enum ATTRIBUTE_UNIFORM_TYPE {
        INT,
        FLOAT,
        MAT3,
        MAT4,
        VEC2,
        VEC3,
        VEC4,

        //Uniform exclusive
        SAMPLER1D,
        SAMPLER2D,
        SAMPLER3D
    };

    GLProgram(std::string path, eShaderType shaderType);
    GLProgram& operator=(const GLProgram& rhs);
    ~GLProgram();

    void create();
    void reset();
    void setUniform(const std::string& uniform, int val);
    void setUniform(const std::string& uniform, float val);
    void setUniform(const std::string& uniform, glm::mat4& val);
    void setUniform(const std::string& uniform, glm::vec3& val);
    void setUniform(const std::string& uniform, glm::vec2& val);
    eShaderType getProgramType();

protected:
    static GLuint getShaderBit(eShaderType type);
    static ATTRIBUTE_UNIFORM_TYPE getAttributeFromGL(GLint type);
    void getShaderInputs();
    void verifyValidUniform(const std::string& uniform, ATTRIBUTE_UNIFORM_TYPE type);

    //reimplementation of glCreateShaderProgramv
    int createShaderProgram(GLenum type, const char** str);
    std::vector<GLuint> m_shaderIds;
    GLuint m_program;
    eShaderType m_shaderType;
    std::string m_path;
    std::unordered_map<std::string, ATTRIBUTE_UNIFORM_TYPE> m_attributes;
    std::unordered_map<std::string, ATTRIBUTE_UNIFORM_TYPE> m_uniforms;
};
