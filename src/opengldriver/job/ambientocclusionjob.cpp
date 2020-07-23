#include "ambientocclusionjob.h"
#include "util/debug.h"
#include "directlightingjob.h"

#include <iostream>
#include <random>

using namespace std;

AmbientOcclusionJob::AmbientOcclusionJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/ambientocclusion.frag");
    generateKernel();
}

AmbientOcclusionJob::~AmbientOcclusionJob()
{
}

void AmbientOcclusionJob::run(ResolveFBO* inFbo, GBufferFBO* normalInput, BlitFBO* outFbo)
{
    outFbo->bind();
    m_pipeline->bindPipeline();
    //using own framebuffer as input and output, so we get weird artifacts
    //m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexture", 0);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uDepth", 1);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uNormal", 2);
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uTexNoise", 3);
    for (int i = 0; i < 64; ++i) {
        m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "samples[" + to_string(i) + "]", m_ssaoKernel[i]);
    }

    GET_GL_ERROR("Error setting uniforms");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inFbo->getAlbedo());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, inFbo->getDepth());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalInput->getNormal());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    GET_GL_ERROR("Error setting uniforms");
    m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "projection", m_proj);
    //m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "viewMatrixInv", m_viewInv);
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

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}
void AmbientOcclusionJob::generateKernel()
{
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i) {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = (float)i / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_ssaoKernel.push_back(sample);
    }
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++) {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }
    //this will never be regenerated
    glGenTextures(1, &m_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
