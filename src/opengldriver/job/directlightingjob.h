#pragma once

#include "job.h"

class DirectLightingJob : public Job
{
public:
	DirectLightingJob();

	void initRTs();
	void run() override;
	void resize(int width, int height) override;
	eRenderPasses getJobType() override { return eRenderPasses::DirectLighting; }

	GLuint getAlbedoRT() { return m_albedo; }
	GLuint getFramebuffer() { return m_framebuffer; } 
private:
	GLuint m_rendertarget;
	GLuint m_framebuffer;

	GLuint m_albedo;
};
