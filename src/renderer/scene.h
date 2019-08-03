#pragma once
#include "mesh.h"
#include <vector>

class Scene {
public:
    ~Scene();
    void createMesh(const char* str);

private:
    std::vector<Mesh*> m_meshes;
};
