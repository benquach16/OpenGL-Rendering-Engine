#pragma once

#include <vector>
#include <glm/glm.hpp>

//what we need to render
struct VertexLayout {
	float x, y, z;
	float nx, ny, nz;
	float tx, ty;
};

struct VertexBuffer {	
	std::vector<VertexLayout> m_vertices;
	std::vector<unsigned int> m_indices;
	
};
