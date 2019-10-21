#include "rendermanager.h"
#include "../../util/debug.h"
#include "../3rdparty/stb_image.h"
#include "job/ambientocclusionjob.h"
#include "job/directlightingjob.h"
#include "job/framebufferjob.h"
#include "job/gbufferjob.h"
#include "job/skyboxjob.h"

using namespace std;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

RenderManager::RenderManager()
    : m_skyboxTexture(0)
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
}

void RenderManager::initRenderPipelines()
{
    loadSkybox();
    //m_renderJobs[eRenderPasses::Shadows] = new Job;
    Job* gbufferJob = new GBufferJob;
    m_renderJobs[eRenderPasses::GBuffer] = gbufferJob;

    DirectLightingJob* directLightingJob = new DirectLightingJob;
    directLightingJob->setParent(gbufferJob);
    ASSERT(m_skyboxTexture != 0, "skybox not loaded");
    directLightingJob->setCubemap(m_skyboxTexture);
    m_renderJobs[eRenderPasses::DirectLighting] = directLightingJob;

    AmbientOcclusionJob* aoJob = new AmbientOcclusionJob;
    aoJob->setParent(directLightingJob);
    m_renderJobs[eRenderPasses::AmbientOcclusion] = aoJob;

    SkyboxJob* skyboxJob = new SkyboxJob;
    skyboxJob->setSkyboxTexture(m_skyboxTexture);
    skyboxJob->setParent(aoJob);
    m_renderJobs[eRenderPasses::Skybox] = skyboxJob;

    //m_renderJobs[eRenderPasses::IndirectLighting] = new Job;
    //m_renderJobs[eRenderPasses::Transparent] = new Job;

    Job* framebufferJob = new FramebufferJob;
    framebufferJob->setParent(skyboxJob);
    m_renderJobs[eRenderPasses::Framebuffer] = framebufferJob;

    m_currentPipeline = 0;
}

void RenderManager::loadSkybox()
{
    GLuint textureId = 0;
    glGenTextures(1, &m_skyboxTexture);
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
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    for (auto i : m_renderJobs) {
        i.second->resize(screenWidth, screenHeight);
    }
}

void RenderManager::render()
{
    // an stl map should guarantee the correct order traversal (smallest enum to largest)
    //int count = 0;
    for (auto it = m_renderJobs.begin(); it != m_renderJobs.end(); ++it) {
        //std::cout << "Running job: " << static_cast<unsigned>(it->first) << " in order " << count << std::endl;
        //count++;
        it->second->run();
    }
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
    static_cast<SkyboxJob*>(m_renderJobs[eRenderPasses::Skybox])->setMVP(skyboxMVP);
}

void sort()
{
    //doesnt do anything right now
}
