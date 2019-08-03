#include "directlightingjob.h"
#include "../../util/debug.h"
#include "gbufferjob.h"

#include <iostream>

using namespace std;

DirectLightingJob::DirectLightingJob()
    : m_framebuffer(0)
    , m_albedo(0)
    , m_cubemap(0)
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/directlighting.frag");
}

void DirectLightingJob::run()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // make sure we have a properly initialized job
    ASSERT(m_width > 0, "Screen Width not set for Direct Lighting Pass");
    ASSERT(m_height > 0, "Screen Height not set for Direct Lighting Pass");

    glViewport(0, 0, m_width, m_height);
    m_pipeline->bindPipeline();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDepth", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uAlbedo", 1);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uNormals", 2);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uCubemap", 3);

    // make sure that our parent is a gbuffer job (DAG strictly enforced)
    ASSERT(m_parent != nullptr, "DAG initialized incorrectly");
    ASSERT(m_parent->getJobType() == eRenderPasses::GBuffer, "Parent job of incorrect type");

    GBufferJob* parent = static_cast<GBufferJob*>(m_parent);
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, parent->getPositionRT());
    GET_GL_ERROR("Error setting position render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, parent->getAlbedoRT());
    GET_GL_ERROR("Error setting albedo render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, parent->getNormalRT());
    GET_GL_ERROR("Error setting normal render target for direct lighting pass");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
    GET_GL_ERROR("Error setting cubemap for direct lighting pass");

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LEQUAL);
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

void DirectLightingJob::resize(int width, int height)
{
    Job::resize(width, height);
    ASSERT(m_parent->getJobType() == eRenderPasses::GBuffer, "Parent job of incorrect type");

    GBufferJob* parent = static_cast<GBufferJob*>(m_parent);

    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glGenTextures(1, &m_albedo);
    glBindTexture(GL_TEXTURE_2D, m_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << parent->getDepthRT() << endl;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, parent->getDepthRT(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedo, 0);

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
