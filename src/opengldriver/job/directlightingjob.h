#pragma once

#include "opengl/resolvefbo.h"
#include "opengl/gbufferfbo.h"
#include "job.h"

class DirectLightingJob : public Job {
public:
    DirectLightingJob();
    ~DirectLightingJob();

    void run(GBufferFBO *inFbo, ResolveFBO *outFbo);
    eRenderPasses getJobType() override { return eRenderPasses::DirectLighting; }
    void setCubemap(GLuint cubemap) { m_cubemap = cubemap; }

private:
    GLuint m_cubemap;

};
