#include "blurjob.h"
#include "util/debug.h"

BlurJob::BlurJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/blur.frag");
}

BlurJob::~BlurJob()
{
}

void BlurJob::run(BlitFBO* inFbo, BlitFBO* outFbo)
{
    outFbo->bind();
    m_pipeline->bindPipeline();

    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uOcclusion", 0);
    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    GET_GL_ERROR("Error setting uniforms");

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
