#include "ambientocclusionjob.h"
#include "../../util/debug.h"
#include "directlightingjob.h"

#include <iostream>

using namespace std;

AmbientOcclusionJob::AmbientOcclusionJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/ambientocclusion.frag");
}

AmbientOcclusionJob::~AmbientOcclusionJob()
{
}

void AmbientOcclusionJob::run()
{
    // make sure we have a properly initialized job
    ASSERT(m_width > 0, "Screen Width not set for Ambient Occlusion Pass");
    ASSERT(m_height > 0, "Screen Height not set for Ambient Occlusion Pass");

    glViewport(0, 0, m_width, m_height);
    m_pipeline->bindPipeline();

    ASSERT(m_parent != nullptr, "DAG initiatized incorrectly");
    ASSERT(m_parent->getJobType() == eRenderPasses::DirectLighting, "Parent job of incorrect type");

    DirectLightingJob* parent = static_cast<DirectLightingJob*>(m_parent);
    //using own framebuffer as input and output, so we get weird artifacts
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, parent->getAlbedoRT());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, parent->getDepthRT());
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDepth", 1);
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

void AmbientOcclusionJob::resize(int width, int height)
{
    Job::resize(width, height);
    ASSERT(m_parent->getJobType() == eRenderPasses::DirectLighting, "Parent job of incorrect type");

    DirectLightingJob* parent = static_cast<DirectLightingJob*>(m_parent);

    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glGenTextures(1, &m_rendertarget);
    glBindTexture(GL_TEXTURE_2D, m_rendertarget);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, parent->getDepthRT(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rendertarget, 0);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
            cerr << "GL_FRAMEBUFFER: incomplete attachment error" << endl;
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
            cerr << "GL_FRAMEBUFFER: incomplete missing attachment" << endl;
        }
        cerr << "framebuffer error" << endl;
    } else {
        cerr << "framebuffer success" << endl;
    }
}

GLuint AmbientOcclusionJob::getFramebuffer() const
{
    return m_framebuffer;
}

GLuint AmbientOcclusionJob::getRT()
{
    return m_rendertarget;
}
