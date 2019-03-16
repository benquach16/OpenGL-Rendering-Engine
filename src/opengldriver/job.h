#pragma once

#include <vector>
#include <queue>
#include "vertexbuffer.h"
#include "opengl3/glpipeline.h"

class Job
{
public:
	Job();
	void init();
	~Job();
	void setVertexShader(const std::string& path);
	void setComputeShader(const std::string& path);
	void setFragmentShader(const std::string& path);
	
	void run();
	void push(VertexBuffer* buf) { m_queue.push(buf); }
protected:
	std::queue<VertexBuffer*> m_queue;
	std::vector<Job*> m_children;

	GLPipeline* m_pipeline;
};
