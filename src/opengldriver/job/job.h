#pragma once

#include <vector>
#include <queue>
#include "vertexbuffer.h"
#include "opengl3/glpipeline.h"

struct RenderTarget
{
	GLuint m_rendertarget;
	int m_width;
	int m_height;
};

enum class eRenderPasses : unsigned
{
	Shadows = 0,
	GBuffer = 1,
	DirectLighting = 2,
	IndirectLighting = 3,
	Transparent = 4
};

class Job
{
public:
	Job();
	virtual ~Job();
	void setVertexShader(const std::string& path);
	void setComputeShader(const std::string& path);
	void setFragmentShader(const std::string& path);
	
	virtual void run();
	virtual void resize(int width, int height) {
		m_width = width;
		m_height = height;
	}

	virtual eRenderPasses getJobType() = 0;
	void push(VertexBuffer* buf) { m_queue.push(buf); }

	void setParent(Job* parent) { m_parent = parent; }
	void addChild(Job* child) { m_children.push_back(child); }
protected:
	int m_width;
	int m_height;

	eRenderPasses m_jobType;
	
	std::queue<VertexBuffer*> m_queue;
	std::vector<Job*> m_children;
	Job* m_parent;
	GLPipeline* m_pipeline;
};
