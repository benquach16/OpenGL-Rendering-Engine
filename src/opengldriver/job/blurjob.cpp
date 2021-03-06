#include "blurjob.h"
#include "util/debug.h"

BlurJob::BlurJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/blur.frag");
}

BlurJob::~BlurJob()
{
}

void BlurJob::run(BlitFBO* inFbo, BlitFBO* outFbo)
{
    outFbo->bind();
    m_pipeline->bindPipeline();

    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uOcclusion", 0);
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
}
