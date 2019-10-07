#include "skyboxjob.h"
#include "../../util/debug.h"
#include "directlightingjob.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../3rdparty/stb_image.h"

using namespace std;

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

SkyboxJob::SkyboxJob() : m_skyboxTexture(0)
{
    setVertexShader("shaders/skybox.vert");
    setFragmentShader("shaders/skybox.frag");
}

SkyboxJob::~SkyboxJob()
{
    m_skyboxTexture = 0;
}

void SkyboxJob::run()
{
    // make sure that our parent is a directlighting job (DAG strictly enforced)
    ASSERT(m_parent != nullptr, "DAG initialized incorrectly");
    ASSERT(m_parent->getJobType() == eRenderPasses::DirectLighting, "Parent job of incorrect type");

    DirectLightingJob* parent = static_cast<DirectLightingJob*>(m_parent);

    glBindFramebuffer(GL_FRAMEBUFFER, parent->getFramebuffer());
    glDepthMask(false);

    m_pipeline->bindPipeline();
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uSkybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
    GET_GL_ERROR("Error setting skybox texture");
    //todo : defer this so we dont alloc memory every frame
    GLuint vertarray;
    glGenVertexArrays(1, &vertarray);
    glBindVertexArray(vertarray);
    GLuint vertices;
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glDepthFunc(GL_LEQUAL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertices);
    glDeleteBuffers(1, &vertarray);

    glDepthMask(true);
}

GLuint SkyboxJob::getRT()
{
    ASSERT(m_parent != nullptr, "DAG initialized incorrectly");
    ASSERT(m_parent->getJobType() == eRenderPasses::DirectLighting, "Parent job of incorrect type");
    DirectLightingJob* parent = static_cast<DirectLightingJob*>(m_parent);
    return parent->getAlbedoRT();
}