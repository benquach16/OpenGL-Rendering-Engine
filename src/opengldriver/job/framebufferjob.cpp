#include "framebufferjob.h"
#include "../../util/debug.h"
#include "directlightingjob.h"
#include "skyboxjob.h"

#include <iostream>

FramebufferJob::FramebufferJob()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/framebuffer.frag");
}

void FramebufferJob::run(ResolveFBO *fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, fbo->getWidth(), fbo->getHeight());

    m_pipeline->bindPipeline();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->getAlbedo());
    GET_GL_ERROR("Error setting position render target in framebuffer job");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // todo : defer this so we dont alloc memory every frame
    GLuint vertarray;
    glGenVertexArrays(1, &vertarray);
    glBindVertexArray(vertarray);
    GLuint vertices;
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(12));

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertices);
    glDeleteBuffers(1, &vertarray);
}
