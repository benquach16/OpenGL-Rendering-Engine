#include "rendermanager.h"

#include "job/gbufferjob.h"
#include "job/directlightingjob.h"

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
	m_renderJobs[eRenderPasses::DirectLighting] = directLightingJob;
	
	//m_renderJobs[eRenderPasses::IndirectLighting] = new Job;
	//m_renderJobs[eRenderPasses::Transparent] = new Job;
	
	m_currentPipeline = 0;	
}

void RenderManager::render()
{
	for(auto i : m_renderJobs)
	{
		i.second->run();
	}
}

void RenderManager::push(VertexBuffer* buf, eRenderPasses renderPass)
{
	m_renderJobs[renderPass]->push(buf);
}


void sort()
{
	//doesnt do anything right now
}
