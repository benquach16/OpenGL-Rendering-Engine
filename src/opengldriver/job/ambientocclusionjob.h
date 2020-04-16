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

    GLuint getRT() const { return m_rendertarget; }
    void generateKernel();
private:
    GLuint m_framebuffer;
    GLuint m_rendertarget;
    GLuint m_noiseTexture;
    std::vector<glm::vec3> m_ssaoKernel;
};
