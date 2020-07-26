#pragma once

#include "job.h"
#include "opengl/blitfbo.h"
#include "opengl/resolvefbo.h"

class HDRJob : public Job {
public:
    HDRJob();
    ~HDRJob();

    void run(ResolveFBO* inFbo, BlitFBO* bloomFbo, BlitFBO* outFbo);

    eRenderPasses getJobType() override { return eRenderPasses::HDR; }

private:
};
