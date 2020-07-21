#pragma once

#include "job.h"
#include "opengl/blitfbo.h"
#include "opengl/resolvefbo.h"

class AmbientOcclusionJob : public Job {
public:
    AmbientOcclusionJob();
    ~AmbientOcclusionJob();

    void run(ResolveFBO* inFbo, GBufferFBO* normalInput, BlitFBO* outFbo);
    void setProj(glm::mat4 projection) { m_proj = projection; }
    void setViewInv(glm::mat4 viewInv) { m_viewInv = m_viewInv; }

    eRenderPasses getJobType() override { return eRenderPasses::AmbientOcclusion; }

    void generateKernel();

private:
    GLuint m_noiseTexture;
    glm::mat4 m_proj;
    glm::mat4 m_viewInv;
    std::vector<glm::vec3> m_ssaoKernel;
};
