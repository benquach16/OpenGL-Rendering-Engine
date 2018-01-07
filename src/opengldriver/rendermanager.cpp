#include "rendermanager.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

void RenderManager::render()
{

	
	while(!m_queue.empty())
	{
		
		auto object = m_queue.front();
		m_queue.pop();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint vertarray;
		glGenVertexArrays(1, &vertarray);
		glBindVertexArray(vertarray);
		GLuint vertices;
		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vertices);
		GLuint indices;
		glGenBuffers(1, &indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.m_indices.size() * sizeof(unsigned int), &object.m_indices[0], GL_STATIC_DRAW);
 
		
		//hardcoded layout for now
		//position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), BUFFER_OFFSET(0));
		//normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), BUFFER_OFFSET(12));
		//texcoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), BUFFER_OFFSET(24));
		
		glBufferData(GL_ARRAY_BUFFER, object.m_vertices.size() * sizeof(VertexLayout), &object.m_vertices[0], GL_STATIC_DRAW);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(
			GL_TRIANGLES,      // mode
			object.m_indices.size(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
			);
		glDisableVertexAttribArray(0);	
	}

}


void sort()
{
	//doesnt do anything right now
}
