#pragma once

#include "job.h"
#include "opengl/blitfbo.h"
#include "opengl/resolvefbo.h"

class FramebufferJob : public Job {
public:
    FramebufferJob();

    void run(BlitFBO* fbo);

    eRenderPasses getJobType() override { return eRenderPasses::Framebuffer; }
};
