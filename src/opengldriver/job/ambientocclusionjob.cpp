#include "ambientocclusionjob.h"

AmbientOcclusionJob::AmbientOcclusionJob() : Job()
{
	setVertexShader("shaders/framebuffer.vert");
	setFragmentShader("shaders/ambientocclusion.frag");
}

void AmbientOcclusionJob::run()
{
}
