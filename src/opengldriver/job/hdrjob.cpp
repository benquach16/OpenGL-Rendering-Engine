#include "hdrjob.h"
#include "util/debug.h"

HDRJob::HDRJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/hdr.frag");
}

HDRJob::~HDRJob()
{
}

void HDRJob::run(ResolveFBO* inFbo, BlitFBO* bloomFbo,  BlitFBO* outFbo)
{
    outFbo->bind();
    m_pipeline->bindPipeline();

    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uBloom", 1);
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
}
