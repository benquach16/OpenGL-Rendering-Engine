#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <queue>
#include "vertexbuffer.h"

class RenderManager
{
public:
	enum RENDER_PASSES
	{
		TRANSPARENT,
		STATIC,
		DYNAMIC
	};
	void render();

	//this function is intended to batch and sort draw calls so that we can maxmimize gpu cache usage
	//we need to group meshes by textures
	//and batch similar meshes
	//we also need to make sure that if we're rendering transparents, we sort them front to back
	void sort();
	void push(VertexBuffer* buf) { m_queue.push(buf); }
private:
	std::queue<VertexBuffer*> m_queue;
	
	
};
