#include "ambientocclusionjob.h"
#include "../../util/debug.h"

#include <iostream>

using namespace std;

AmbientOcclusionJob::AmbientOcclusionJob()
    : Job()
{
    setVertexShader("shaders/framebuffer.vert");
    setFragmentShader("shaders/ambientocclusion.frag");
}

AmbientOcclusionJob::~AmbientOcclusionJob()
{
}

void AmbientOcclusionJob::run()
{
    // make sure we have a properly initialized job
    ASSERT(m_width > 0, "Screen Width not set for Ambient Occlusion Pass");
    ASSERT(m_height > 0, "Screen Height not set for Ambient Occlusion Pass");

    glViewport(0, 0, m_width, m_height);

    ASSERT(m_parent != nullptr, "DAG initiatized incorrectly");
}

void AmbientOcclusionJob::resize(int width, int height)
{
    Job::resize(width, height);
}
