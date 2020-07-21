#pragma once

#include "job.h"
#include "opengl/gbufferfbo.h"

#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

class GBufferJob : public Job {
public:
    GBufferJob();
    ~GBufferJob();
    void resetRTs();
    void run(GBufferFBO* fbo);

    void setMVP(const glm::mat4& MVP);
    void setView(const glm::mat4& view);

    eRenderPasses getJobType() override { return eRenderPasses::GBuffer; }
};
