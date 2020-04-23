#pragma once

#include "opengl/resolvefbo.h"
#include "job.h"

class SkyboxJob : public Job {
public:
    SkyboxJob();
    ~SkyboxJob() override;

    void setMVP(const glm::mat4& MVP) { m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP); }

    void setSkyboxTexture(GLuint skyboxTexture) { m_skyboxTexture = skyboxTexture; }
    void run(ResolveFBO *fbo);
    eRenderPasses getJobType() override { return eRenderPasses::Skybox; }

    GLuint getRT();

private:
    GLuint m_skyboxTexture;
};
