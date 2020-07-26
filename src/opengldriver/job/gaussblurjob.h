#pragma once

#include "job.h"
#include "opengl/blitfbo.h"

class GaussBlurJob : public Job {
public:
    GaussBlurJob();
    ~GaussBlurJob();

    void run(BlitFBO* inOutFbo, BlitFBO* tempFbo, float radius);

    eRenderPasses getJobType() override { return eRenderPasses::Blur; }
};