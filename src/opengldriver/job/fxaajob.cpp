#include "fxaajob.h"
#include "util/debug.h"

FXAAJob::FXAAJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/fxaa.frag");
}

FXAAJob::~FXAAJob()
{
}

void FXAAJob::run(BlitFBO* inFbo, BlitFBO* outFbo)
{
    outFbo->bind();
    m_pipeline->bindPipeline();

    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
}
