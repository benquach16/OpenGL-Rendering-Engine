#include "directlightingjob.h"

DirectLightingJob::DirectLightingJob()
{
	setVertexShader("shaders/framebuffer.vert");
	setFragmentShader("shaders/cooktorrance.frag");
}

void DirectLightingJob::run()
{
}
