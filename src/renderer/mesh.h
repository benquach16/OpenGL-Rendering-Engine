#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>

//define vertex layout
//vertex_position
//vertex_normals
//tex_coordinates
struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float tx, ty;
};

class Mesh
{
public:
    Mesh();
	
	void load(const char* str);
	void load(std::vector<Vertex> vertices);
	void render();

private:
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;
	
};
