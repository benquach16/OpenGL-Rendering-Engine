#pragma once

#include "opengl/resolvefbo.h"
#include "opengl/blitfbo.h"
#include "job.h"

class FramebufferJob : public Job {
public:
    FramebufferJob();

    void run(BlitFBO *fbo);

    eRenderPasses getJobType() override { return eRenderPasses::Framebuffer; }
};
