#pragma once

#include "opengl/resolvefbo.h"
#include "opengl/blitfbo.h"
#include "job.h"

class AmbientOcclusionJob : public Job {
public:
    AmbientOcclusionJob();
    ~AmbientOcclusionJob();

    void run(ResolveFBO *inFbo, GBufferFBO *normalInput, BlitFBO *outFbo);
    void resize(int width, int height);
    void setProj(glm::mat4 projection) { m_proj = projection; }
    void setViewInv(glm::mat4 viewInv) { m_viewInv = m_viewInv; }

    eRenderPasses getJobType() override { return eRenderPasses::AmbientOcclusion; }
    GLuint getFramebuffer() const;

    GLuint getRT() const { return m_rendertarget; }
    void generateKernel();
private:
    GLuint m_framebuffer;
    GLuint m_rendertarget;
    GLuint m_noiseTexture;
    glm::mat4 m_proj;
    glm::mat4 m_viewInv;
    std::vector<glm::vec3> m_ssaoKernel;
};
