#pragma once

#include "job.h"

class SkyboxJob : public Job
{
public:
	SkyboxJob();
	~SkyboxJob() override;

	void run() override;
	eRenderPasses getJobType() override { return eRenderPasses::Skybox; }
private:
	GLuint m_skyboxTexture;
};
