#pragma once

#include <vector>
#include "../opengldriver/vertexbuffer.h"

class Mesh
{
public:
    Mesh();
	
	void load(const char* str);
	void render();

private:
	VertexBuffer m_vertices;
};
