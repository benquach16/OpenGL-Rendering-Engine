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
	std::cerr << "loaded file :" << str << " with " << loader.LoadedMeshes.size() << " meshes" << std::endl;
	
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
	

/*	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, str,
								0, true);
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				
				tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
				tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
				tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
				tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
				tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
				tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
				//tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
				//tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

				VertexLayout layout;
				layout.x = vx;
				layout.y = vy;
				layout.z = vz;
				layout.nx = nx;
				layout.ny = ny;
				layout.nz = nz;
				//layout.tx = tx;
				//layout.ty = ty;

				m_vertexbuffer->m_vertices.push_back(layout);
			}
			index_offset += fv;
		}
	}
*/	
}


void Mesh::render()
{
}
