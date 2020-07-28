#pragma once

#include "job.h"
#include "opengl/blitfbo.h"

class FXAAJob : public Job {
public:
    FXAAJob();
    ~FXAAJob();
    void run(BlitFBO* inFBO, BlitFBO* outFBO);

    eRenderPasses getJobType() override { return eRenderPasses::FXAA; }

private:
};
