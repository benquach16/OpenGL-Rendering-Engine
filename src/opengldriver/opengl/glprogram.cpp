#include "glprogram.h"
#include "../../util/debug.h"

using namespace std;

GLProgram::GLProgram(std::string path, eShaderType shaderType)
    : m_program(0)
    , m_shaderType(shaderType)
    , m_path(path)
{
    create();
}

GLProgram& GLProgram::operator=(const GLProgram& rhs)
{
    reset();
    if (rhs.m_program) {
        this->m_program = rhs.m_program;
    }
    this->m_uniforms = rhs.m_uniforms;
    this->m_shaderType = rhs.m_shaderType;
    return *this;
}

GLProgram::~GLProgram()
{
    reset();
}

void GLProgram::create()
{
    reset();
    std::string contents = util::loadFile(m_path);
    const char* ptr = contents.c_str();
    //auto id = glCreateShaderProgramv(getShaderBit(m_shaderType), 1, &ptr);
    auto id = createShaderProgram(getShaderBit(m_shaderType), &ptr);
    ASSERT(id, "invalid shaderprogram generated");
    GET_GL_ERROR("error creating shaderprogram with shader " + m_path);
    m_program = id;
    getShaderInputs();
}

void GLProgram::reset()
{
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void GLProgram::setUniform(const std::string& uniform, int val)
{
    verifyValidUniform(uniform, ATTRIBUTE_UNIFORM_TYPE::INT);
    auto location = glGetUniformLocation(m_program, uniform.c_str());
    GET_GL_ERROR("GLProgram: Uniform Error when assigning to " + uniform + " in shader " + m_path);
    glProgramUniform1i(m_program, location, GLint(val));
}

void GLProgram::setUniform(const std::string& uniform, float val)
{
    verifyValidUniform(uniform, ATTRIBUTE_UNIFORM_TYPE::FLOAT);
    auto location = glGetUniformLocation(m_program, uniform.c_str());
    GET_GL_ERROR("GLProgram: Uniform Error when assigning to " + uniform + " in shader " + m_path);
    glProgramUniform1f(m_program, location, GLfloat(val));
}

void GLProgram::setUniform(const std::string& uniform, glm::mat4& val)
{
    verifyValidUniform(uniform, ATTRIBUTE_UNIFORM_TYPE::MAT4);
    auto location = glGetUniformLocation(m_program, uniform.c_str());
    GET_GL_ERROR("GLProgram: Uniform Error when assigning to " + uniform + " in shader " + m_path);
    glProgramUniformMatrix4fv(m_program, location, 1, false, glm::value_ptr(val));
}

void GLProgram::setUniform(const std::string& uniform, glm::vec3& val)
{
}

//create our own glCreateShaderProgram function because we want our own logging
int GLProgram::createShaderProgram(GLenum type, const char** str)
{
    const GLuint shader = glCreateShader(type);
    if (shader) {
        glShaderSource(shader, 1, (const GLchar**)str, NULL);
        glCompileShader(shader);
        const GLuint program = glCreateProgram();
        if (program) {
            GLint compiled = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
            if (compiled) {
                glAttachShader(program, shader);
                glLinkProgram(program);
                glDetachShader(program, shader);
            } else {
                cerr << "Error: shader compilation for " << m_path << endl;
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> errorLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
                for (auto i : errorLog) {
                    cerr << i;
                }
            }
            /* append-shader-info-log-to-program-info-log */
        }
        glDeleteShader(shader);
        return program;
    } else {
        return 0;
    }
}

GLuint GLProgram::getShaderBit(GLProgram::eShaderType type)
{
    switch (type) {
    case GLProgram::eShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case GLProgram::eShaderType::Tesselation:
        return GL_TESS_CONTROL_SHADER;
    case GLProgram::eShaderType::Geometry:
        return GL_GEOMETRY_SHADER;
    case GLProgram::eShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    }
    return -1;
}

GLProgram::ATTRIBUTE_UNIFORM_TYPE GLProgram::getAttributeFromGL(GLint type)
{
    switch (type) {
    case GL_INT:
        return INT;
    case GL_FLOAT:
        return FLOAT;
    case GL_FLOAT_VEC2:
        return VEC2;
    case GL_FLOAT_VEC3:
        return VEC3;
    case GL_FLOAT_VEC4:
        return VEC4;
    case GL_FLOAT_MAT3:
        return MAT3;
    case GL_FLOAT_MAT4:
        return MAT4;
    }
    return INT;
}

void GLProgram::getShaderInputs()
{

    //get shader inputs
    GLint numActiveAttribs = 0;
    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(m_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
    glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());
    cout << m_path << " has " << numActiveAttribs << " attributes and " << numActiveUniforms << " uniforms." << endl;
    for (int i = 0; i < numActiveAttribs; ++i) {
        glGetProgramResourceiv(m_program, GL_PROGRAM_INPUT, i, properties.size(),
            &properties[0], values.size(), NULL, &values[0]);

        nameData.resize(values[0]); //The length of the name.
        glGetProgramResourceName(m_program, GL_PROGRAM_INPUT, i, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);
        cout << m_path << " found input " << name << endl;
        auto type = values[1];
        m_attributes[name] = getAttributeFromGL(type);
    }

    for (int i = 0; i < numActiveUniforms; ++i) {
        glGetProgramResourceiv(m_program, GL_UNIFORM, i, properties.size(),
            &properties[0], values.size(), NULL, &values[0]);

        nameData.resize(values[0]); //The length of the name.
        glGetProgramResourceName(m_program, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);
        cout << m_path << " found uniform " << name << endl;
        auto type = values[1];
        m_uniforms[name] = getAttributeFromGL(type);
    }
}

void GLProgram::verifyValidUniform(const std::string& uniform, ATTRIBUTE_UNIFORM_TYPE type)
{
    ASSERT(m_program, "attempted to set uniform on uninitialized program");
    auto it = m_uniforms.find(uniform);
    if (it == m_uniforms.end()) {
        cerr << "attempted to set unknown uniform " << uniform << endl;
        return;
    }
    if (it->second != type) {
        cerr << "attempted to set incorrect uniform type" << endl;
        return;
    }
}
