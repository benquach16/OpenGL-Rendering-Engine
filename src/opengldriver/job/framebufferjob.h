#pragma once

#include "job.h"

class FramebufferJob : public Job
{
public:
	FramebufferJob();

	void run() override;

	eRenderPasses getJobType() override { return eRenderPasses::Framebuffer; }
};
