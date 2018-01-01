#include "mesh.h"

Mesh::Mesh()
{

}


void Mesh::load(const char* str)
{
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

	
}
