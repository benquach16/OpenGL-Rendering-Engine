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

void DirectLightingJob::run(GBufferFBO* inFbo, BlitFBO* blurFbo, ResolveFBO* outFbo)
{
    outFbo->bind();
    glViewport(0, 0, outFbo->getWidth(), outFbo->getHeight());
    m_pipeline->bindPipeline();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uPosition", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uAlbedo", 1);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uNormals", 2);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uOcclusion", 3);
    //m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uCubemap", 4);

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
    glBindTexture(GL_TEXTURE_2D, blurFbo->getAlbedo());
    GET_GL_ERROR("Error setting ao render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
    GET_GL_ERROR("Error setting cubemap for direct lighting pass");

    // todo : defer this so we dont alloc memory every frame
    renderQuad();

    glDepthMask(GL_TRUE);
}
