#include "directlightingjob.h"
#include "../../util/debug.h"
#include "gbufferjob.h"

#include <iostream>

using namespace std;

DirectLightingJob::DirectLightingJob()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/directlighting.frag");
}

DirectLightingJob::~DirectLightingJob()
{
}

void DirectLightingJob::run(GBufferFBO *inFbo, ResolveFBO *outFbo)
{
    outFbo->bind();
    glViewport(0, 0, outFbo->getWidth(), outFbo->getHeight());
    m_pipeline->bindPipeline();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uPosition", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uAlbedo", 1);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uNormals", 2);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uCubemap", 3);
    
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getPosition());
    GET_GL_ERROR("Error setting position render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    GET_GL_ERROR("Error setting albedo render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, inFbo->getNormal());
    GET_GL_ERROR("Error setting normal render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
    GET_GL_ERROR("Error setting cubemap for direct lighting pass");

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

    glDepthMask(GL_TRUE);
}

