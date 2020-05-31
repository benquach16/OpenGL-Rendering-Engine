#pragma once

#include "opengl/skyboxfbo.h"
#include "opengl/resolvefbo.h"
#include "job.h"

class FramebufferJob : public Job {
public:
    FramebufferJob();

    void run(ResolveFBO *fbo);

    eRenderPasses getJobType() override { return eRenderPasses::Framebuffer; }
};
