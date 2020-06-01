#pragma once

#include "opengl3/glpipeline.h"
#include "vertexbuffer.h"
#include <queue>
#include <vector>

struct RenderTarget {
    GLuint m_rendertarget;
    int m_width;
    int m_height;
};

enum class eRenderPasses : unsigned {
    Shadows = 0,
    GBuffer = 1,
    DirectLighting = 2,
    IndirectLighting = 3,
    AmbientOcclusion = 4,
    Skybox = 5,
    Transparent = 6,
    Framebuffer = 7,
};

//0: vertex pos
//1: texcoord
// todo: move me!
static GLfloat quad[] = {
    //upper left triangle
    -1.0f, -1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    0.0f, 1.0f,

    //bottom right triangle
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 0.0f
};

class Job {
public:
    Job();
    virtual ~Job();
    void setVertexShader(const std::string& path);
    void setComputeShader(const std::string& path);
    void setFragmentShader(const std::string& path);

    void resize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    virtual eRenderPasses getJobType() = 0;
    void push(VertexBuffer* buf) { m_queue.push(buf); }


protected:
    int m_width;
    int m_height;

    eRenderPasses m_jobType;

    std::queue<VertexBuffer*> m_queue;
    GLPipeline* m_pipeline;
};
