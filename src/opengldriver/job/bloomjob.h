#pragma once

#include "job.h"
#include "opengl/blitfbo.h"
#include "opengl/resolvefbo.h"

class BloomJob : public Job {
public:
    BloomJob();
    ~BloomJob();

    eRenderPasses getJobType() { return eRenderPasses::Bloom; }

    void run(ResolveFBO* inFbo, BlitFBO* outFbo);
};