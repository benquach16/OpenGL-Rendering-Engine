#include "rendermanager.h"
#include "../../util/debug.h"
#include "../3rdparty/stb_image.h"
#include "job/ambientocclusionjob.h"
#include "job/blurjob.h"
#include "job/directlightingjob.h"
#include "job/framebufferjob.h"
#include "job/gbufferjob.h"
#include "job/skyboxjob.h"

using namespace std;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

RenderManager::RenderManager()
    : m_skyboxTexture(0)
    , m_gbufferFBO(nullptr)
    , m_resolveFBO(nullptr)
{
}

RenderManager::~RenderManager()
{
    for (auto i : m_renderPipelines) {
        delete i.second;
    }
    m_renderPipelines.clear();

    for (auto i : m_renderJobs) {
        delete i.second;
    }
    m_renderJobs.clear();

    if (m_gbufferFBO != nullptr) {
        delete m_gbufferFBO;
        m_gbufferFBO = nullptr;
    }

    if (m_resolveFBO != nullptr) {
        delete m_resolveFBO;
        m_resolveFBO = nullptr;
    }
}

void RenderManager::initRenderPipelines()
{
    m_gbufferFBO = new GBufferFBO;
    m_resolveFBO = new ResolveFBO(m_gbufferFBO);
    m_aoFBO = new BlitFBO;
    m_blurFBO = new BlitFBO;
    loadSkybox();
    //m_renderJobs[eRenderPasses::Shadows] = new Job;
    Job* gbufferJob = new GBufferJob;
    m_renderJobs[eRenderPasses::GBuffer] = gbufferJob;

    DirectLightingJob* directLightingJob = new DirectLightingJob;
    ASSERT(m_skyboxTexture != 0, "skybox not loaded");
    directLightingJob->setCubemap(m_skyboxTexture);
    m_renderJobs[eRenderPasses::DirectLighting] = directLightingJob;

    AmbientOcclusionJob* aoJob = new AmbientOcclusionJob;
    m_renderJobs[eRenderPasses::AmbientOcclusion] = aoJob;

    BlurJob* blurJob = new BlurJob;
    m_renderJobs[eRenderPasses::AOBlur] = blurJob;

    SkyboxJob* skyboxJob = new SkyboxJob;
    skyboxJob->setSkyboxTexture(m_skyboxTexture);
    m_renderJobs[eRenderPasses::Skybox] = skyboxJob;

    Job* framebufferJob = new FramebufferJob;
    m_renderJobs[eRenderPasses::Framebuffer] = framebufferJob;

    m_currentPipeline = 0;
}

void RenderManager::loadSkybox()
{
    GLuint textureId = 0;
    glGenTextures(1, &m_skyboxTexture);
    //funny thing, if you keep this bound it causes the skybox job to work even though the cubemap uniform hasn't been set correctly
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    vector<std::string> faces = {
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    };
    int width, height, nrChannels;
    for (unsigned i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void RenderManager::resize(int screenWidth, int screenHeight)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_gbufferFBO->resize(m_screenWidth, m_screenHeight);
    m_resolveFBO->resize(m_screenWidth, m_screenHeight);
    m_aoFBO->resize(m_screenWidth, m_screenHeight);
    m_blurFBO->resize(m_screenWidth, m_screenHeight);
}

void RenderManager::render()
{
    static_cast<GBufferJob*>(m_renderJobs[eRenderPasses::GBuffer])->run(m_gbufferFBO);
    static_cast<DirectLightingJob*>(m_renderJobs[eRenderPasses::DirectLighting])->run(m_gbufferFBO, m_resolveFBO);
    static_cast<SkyboxJob*>(m_renderJobs[eRenderPasses::Skybox])->run(m_resolveFBO);
    static_cast<AmbientOcclusionJob*>(m_renderJobs[eRenderPasses::AmbientOcclusion])->run(m_resolveFBO, m_gbufferFBO, m_aoFBO);
    static_cast<BlurJob*>(m_renderJobs[eRenderPasses::AOBlur])->run(m_aoFBO, m_blurFBO);
    static_cast<FramebufferJob*>(m_renderJobs[eRenderPasses::Framebuffer])->run(m_blurFBO);
}

void RenderManager::push(VertexBuffer* buf, eRenderPasses renderPass)
{
    m_renderJobs[renderPass]->push(buf);
}

void RenderManager::setCameraPerspective(const glm::mat4& view, const glm::mat4& projection)
{
    //add jobs that need the MVP here

    glm::mat4 viewNoPos = view;
    viewNoPos[3][2] = 0.0f;
    viewNoPos[3][1] = 0.0f;
    viewNoPos[3][0] = 0.0f;
    glm::mat4 skyboxMVP = projection * viewNoPos;
    glm::mat4 MVP = projection * view;
    static_cast<GBufferJob*>(m_renderJobs[eRenderPasses::GBuffer])->setMVP(MVP);
    static_cast<GBufferJob*>(m_renderJobs[eRenderPasses::GBuffer])->setView(view);
    static_cast<SkyboxJob*>(m_renderJobs[eRenderPasses::Skybox])->setMVP(skyboxMVP);

    glm::mat4 projectionInv = glm::inverse(projection);
    glm::mat4 viewInv = glm::inverse(view);
    static_cast<AmbientOcclusionJob*>(m_renderJobs[eRenderPasses::AmbientOcclusion])->setProj(projection);
    static_cast<AmbientOcclusionJob*>(m_renderJobs[eRenderPasses::AmbientOcclusion])->setViewInv(viewInv);
}

void sort()
{
    //doesnt do anything right now
}
