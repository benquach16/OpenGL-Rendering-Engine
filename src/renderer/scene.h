#pragma once
#include <vector>
#include "mesh.h"

class Scene
{
public:
	~Scene();
	void createMesh(const char* str);
private:
	std::vector<Mesh*> m_meshes;
};
