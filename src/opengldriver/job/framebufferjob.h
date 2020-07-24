#pragma once

#include "job.h"
#include "opengl/blitfbo.h"
#include "opengl/resolvefbo.h"

class FramebufferJob : public Job {
public:
    FramebufferJob();

    void run(ResolveFBO* fbo);

    eRenderPasses getJobType() override { return eRenderPasses::Framebuffer; }
};
