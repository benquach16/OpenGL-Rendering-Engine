#include "mesh.h"
#include "../3rdparty/OBJ_Loader.h"
#include <iostream>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

Mesh::Mesh()
{

}


void Mesh::load(const char* str)
{
	objl::Loader loader;
	loader.LoadFile(str);
	
}

void Mesh::load(std::vector<Vertex> vertices)
{
	
}

void Mesh::render()
{
	//vertex_position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));

	//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));

	//tex_coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));	
}
