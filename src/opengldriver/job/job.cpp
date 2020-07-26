#include "job.h"

Job::Job()
    : m_pipeline(nullptr)
    , m_width(-1)
    , m_height(-1)
{
    m_pipeline = new GLPipeline;
}

Job::~Job()
{
    if (m_pipeline != nullptr) {
        delete m_pipeline;
    }

    m_pipeline = nullptr;
}

void Job::setVertexShader(const std::string& path)
{
    m_pipeline->addShader(path, GLProgram::eShaderType::Vertex);
}

void Job::setFragmentShader(const std::string& path)
{
    m_pipeline->addShader(path, GLProgram::eShaderType::Fragment);
}

void Job::renderQuad()
{
    // todo : defer this so we dont alloc memory every frame
    GLuint vertarray;
    glGenVertexArrays(1, &vertarray);
    glBindVertexArray(vertarray);
    GLuint vertices;
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(12));

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertices);
    glDeleteBuffers(1, &vertarray);
}
