#pragma once

#include "job.h"

class SkyboxJob : public Job
{
public:
	SkyboxJob();
	~SkyboxJob() override;

	void run() override;
private:
	GLuint m_skyboxTexture;
};
