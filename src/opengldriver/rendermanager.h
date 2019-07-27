#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <queue>
#include <map>
#include "vertexbuffer.h"
#include "renderstate.h"
#include "opengl/glprogram.h"
#include "opengl3/glpipeline.h"
#include "job/job.h"

enum eRenderPipelines
{
	Deferred,
	PostProcess,
	Framebuffer,
};

class RenderManager
{
public:	
	RenderManager();
	~RenderManager();
	void initRenderPipelines();
	void renderLightVolume();
	void render();
	void resize(int screenWidth, int screeHeight);
	void renderDeferred();
	void renderForward();
	//this function is intended to batch and sort draw calls so that we can maxmimize gpu cache usage
	//we need to group meshes by textures
	//and batch similar meshes
	//we also need to make sure that if we're rendering transparents, we sort them front to back
	void sort();
	void push(VertexBuffer* buf) { m_queue.push(buf); }
	void push(VertexBuffer* buf, eRenderPasses renderPass);

	void setCameraPerspective(const glm::mat4 &view, const glm::mat4 &projection);
private:
	
	Job* m_root;
	
	std::queue<VertexBuffer*> m_queue;
	std::vector<RenderState> m_renderStates;
	std::map<eRenderPasses, Job*> m_renderJobs;
	std::unordered_map<eRenderPipelines, GLPipeline*> m_renderPipelines;

	unsigned m_currentRenderstate;
	unsigned m_currentPipeline;

	std::vector<GLuint> m_framebuffers;

	int m_screenWidth;
	int m_screenHeight;
};
