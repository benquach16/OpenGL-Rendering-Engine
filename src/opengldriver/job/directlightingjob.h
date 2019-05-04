#pragma once

#include "job.h"

class DirectLightingJob : public Job
{
public:
	DirectLightingJob();

	void run() override;
	eRenderPasses getJobType() override { return eRenderPasses::DirectLighting; }
private:
};
