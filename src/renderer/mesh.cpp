#include "mesh.h"
#include "../3rdparty/OBJ_Loader.h"
#include <iostream>

using namespace std;


Mesh::Mesh()
{

}


void Mesh::load(const char* str)
{
	objl::Loader loader;
	loader.LoadFile(str);
	for(int i = 0; i < loader.LoadedMeshes[0].Vertices.size(); ++i)
	{
		auto vert = loader.LoadedMeshes[0].Vertices[i];
		float x, y, z;
		float nx, ny, nz;
		float tx, ty;

		x = vert.Position.X;
		y = vert.Position.Y;
		z = vert.Position.Z;

		nx = vert.Normal.X;
		ny = vert.Normal.Y;
		nz = vert.Normal.Z;

		
	}
}


void Mesh::render()
{
}
