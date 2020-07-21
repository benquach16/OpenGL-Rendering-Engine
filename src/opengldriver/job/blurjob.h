#pragma once

#include "job.h"
#include "opengl/blitfbo.h"

class BlurJob : public Job {
public:
    BlurJob();
    ~BlurJob();
    void run(BlitFBO* inFBO, BlitFBO* outFBO);

    eRenderPasses getJobType() override { return eRenderPasses::AOBlur; }

private:
};
