#include "blurjob.h"

BlurJob::BlurJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/ambientocclusion.frag");
}

BlurJob::~BlurJob()
{
    
}

void BlurJob::run(BlitFBO* inFBO, BlitFBO* outFBO)
{
    m_pipeline->bindPipeline();
}
