#pragma once

#include "job.h"

class DirectLightingJob : public Job {
public:
    DirectLightingJob();
    ~DirectLightingJob();

    void initRTs();
    void run() override;
    void resize(int width, int height) override;
    eRenderPasses getJobType() override { return eRenderPasses::DirectLighting; }
    void setCubemap(GLuint cubemap) { m_cubemap = cubemap; }
    GLuint getAlbedoRT() { return m_albedo; }
    GLuint getFramebuffer() { return m_framebuffer; }

private:
    GLuint m_cubemap;

    GLuint m_rendertarget;
    GLuint m_framebuffer;

    GLuint m_albedo;
};
