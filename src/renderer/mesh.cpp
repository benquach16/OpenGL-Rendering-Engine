#include "mesh.h"
#include "../3rdparty/OBJ_Loader.h"
#include <iostream>

using namespace std;


Mesh::Mesh()
{
	m_vertexbuffer = new VertexBuffer;
}

Mesh::~Mesh()
{
	delete m_vertexbuffer;
}


void Mesh::load(const char* str)
{
	objl::Loader loader;
	loader.LoadFile(str);
	for(int i = 0; i < loader.LoadedMeshes[0].Vertices.size(); ++i)
	{
		auto vert = loader.LoadedMeshes[0].Vertices[i];		
		VertexLayout layout;
		layout.x = vert.Position.X;
		layout.y = vert.Position.Y;
		layout.z = vert.Position.Z;

		layout.nx = vert.Normal.X;
		layout.ny = vert.Normal.Y;
		layout.nz = vert.Normal.Z;

		layout.tx = vert.TextureCoordinate.X;
		layout.ty = vert.TextureCoordinate.Y;

		m_vertexbuffer->m_vertices.push_back(layout);
	}
	m_vertexbuffer->m_indices = loader.LoadedMeshes[0].Indices;
}


void Mesh::render()
{
}
