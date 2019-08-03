#include "ambientocclusionjob.h"

AmbientOcclusionJob::AmbientOcclusionJob() : Job()
{
	setVertexShader("shaders/framebuffer.vert");
	setFragmentShader("shaders/ambientocclusion.frag");
}

AmbientOcclusionJob::~AmbientOcclusionJob()
{
}

void AmbientOcclusionJob::run()
{
}

void AmbientOcclusionJob::resize(int width, int height)
{
    Job::resize(width, height);
}
