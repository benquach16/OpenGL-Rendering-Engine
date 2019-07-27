#pragma once

#include "job.h"

class SkyboxJob : public Job
{
public:
	SkyboxJob();
	~SkyboxJob() override;

	void setMVP(const glm::mat4 &MVP) {	m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP); }
	
	void run() override;
	eRenderPasses getJobType() override { return eRenderPasses::Skybox; }
private:
	GLuint m_skyboxTexture;
};
