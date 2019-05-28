#include "rendermanager.h"

#include "job/gbufferjob.h"
#include "job/directlightingjob.h"
#include "job/skyboxjob.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

RenderManager::RenderManager() 
{
}

RenderManager::~RenderManager()
{
	for(auto i : m_renderPipelines)
	{
		delete i.second;
	}
	m_renderPipelines.clear();

	for(auto i : m_renderJobs)
	{
		delete i.second;
	}
	m_renderJobs.clear();
}

void RenderManager::initRenderPipelines()
{
	//m_renderJobs[eRenderPasses::Shadows] = new Job;
	Job* gbufferJob = new GBufferJob;
	m_renderJobs[eRenderPasses::GBuffer] = gbufferJob;

	Job* directLightingJob = new DirectLightingJob;
	directLightingJob->setParent(gbufferJob);
	m_renderJobs[eRenderPasses::DirectLighting] = directLightingJob;

	//Job* skyboxJob = new SkyboxJob;
	//m_renderJobs[eRenderPasses::Skybox] = skyboxJob;

	//m_renderJobs[eRenderPasses::IndirectLighting] = new Job;
	//m_renderJobs[eRenderPasses::Transparent] = new Job;
	
	m_currentPipeline = 0;	
}

void RenderManager::resize(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	for(auto i : m_renderJobs)
	{
		i.second->resize(screenWidth, screenHeight);
	}	
}

void RenderManager::render()
{
	// an stl map should guarantee the correct order traversal (smallest enum to largest)
	//int count = 0;
	for(auto it = m_renderJobs.begin(); it != m_renderJobs.end(); ++it )
	{
		//std::cout << "Running job: " << static_cast<unsigned>(it->first) << " in order " << count << std::endl;
		//count++;
		it->second->run();
	}
}

void RenderManager::push(VertexBuffer* buf, eRenderPasses renderPass)
{
	m_renderJobs[renderPass]->push(buf);
}

void RenderManager::setCameraPerspective(const glm::mat4 &MVP)
{
	static_cast<GBufferJob*>(m_renderJobs[eRenderPasses::GBuffer])->setMVP(MVP);
}


void sort()
{
	//doesnt do anything right now
}
