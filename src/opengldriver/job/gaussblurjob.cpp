#include "gaussblurjob.h"
#include "util/debug.h"

GaussBlurJob::GaussBlurJob()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/gaussblur.frag");
}

GaussBlurJob::~GaussBlurJob()
{
}

void GaussBlurJob::run(BlitFBO* inOutFbo, BlitFBO* tempFbo, float radius)
{
    tempFbo->bind();
    m_pipeline->bindPipeline();

    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDirection", glm::vec2(radius, 0.0));
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inOutFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();

    inOutFbo->bind();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDirection", glm::vec2(0.0, radius));
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tempFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
}
