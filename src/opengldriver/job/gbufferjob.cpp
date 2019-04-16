#include "gbufferjob.h"
#include "../../util/util.h"

using namespace std;

GBufferJob::GBufferJob() : Job()
{
	setVertexShader("shaders/gbuffer.vert");
	setFragmentShader("shaders/gbuffer.frag");
}

void GBufferJob::initRTs(int width, int height)
{
	glGenFramebuffers(1, &m_gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &m_position);
	glBindTexture(GL_TEXTURE_2D, m_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glGenTextures(1, &m_albedo);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &m_normals);
	glBindTexture(GL_TEXTURE_2D, m_normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_position, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_albedo, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_normals, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, DrawBuffers); // "1" is the size of DrawBuffers
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		{
			cerr << "GL_FRAMEBUFFER: incomplete attachment error" << endl;
		}
		if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		{
			cerr << "GL_FRAMEBUFFER: incomplete missing attachment" << endl;
		}
		cerr << "framebuffer error" << endl;
	}
	else
	{
		cerr << "framebuffer success" << endl;
	}
}

void GBufferJob::setMVP(const glm::mat4 &MVP)
{
	m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP);
	
}

void GBufferJob::run()
{
	m_pipeline->bindPipeline();
	while(!m_queue.empty())
	{
		auto object = m_queue.front();
		m_queue.pop();

		auto index_size = object->m_indices.size();
		auto vertex_size = object->m_vertices.size();
		if(!vertex_size)
		{
			//nothing to render
			continue;
		}
			
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint vertarray;
		glGenVertexArrays(1, &vertarray);
		glBindVertexArray(vertarray);
		GLuint vertices;
		glGenBuffers(1, &vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vertices);
		GLuint indices;
		if(index_size)
		{
			glGenBuffers(1, &indices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->m_indices.size() * sizeof(unsigned int), &object->m_indices[0], GL_STATIC_DRAW);
		} 
		
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
		
		glBufferData(GL_ARRAY_BUFFER, object->m_vertices.size() * sizeof(VertexLayout), &object->m_vertices[0], GL_STATIC_DRAW);

		if(!index_size)
		{
			//generate identity index buffer if no indices found
			glDrawArrays(GL_TRIANGLES, 0, vertex_size);
			glDeleteBuffers(1, &vertices);
		}
		else
		{
			//draw elements properly
			glDrawElements(
			GL_TRIANGLES,      // mode
			index_size,    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
			);
			glDeleteBuffers(1, &indices);
			glDeleteBuffers(1, &vertices);
		}
		glDisableVertexAttribArray(0);
	}
}
