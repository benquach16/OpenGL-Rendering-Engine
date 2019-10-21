#pragma once

#include "job.h"

class AmbientOcclusionJob : public Job {
public:
    AmbientOcclusionJob();
    ~AmbientOcclusionJob();

    void run() override;
    void resize(int width, int height) override;

    eRenderPasses getJobType() override { return eRenderPasses::AmbientOcclusion; }
    GLuint getFramebuffer() const;

    GLuint getRT();
private:
    GLuint m_framebuffer;
};
