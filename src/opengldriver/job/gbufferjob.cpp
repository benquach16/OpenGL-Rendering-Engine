#include "gbufferjob.h"
#include "../../util/debug.h"
#include "../../util/util.h"

using namespace std;

GBufferJob::GBufferJob()
    : Job()
{
    setVertexShader("shaders/gbuffer.vert");
    setFragmentShader("shaders/gbuffer.frag");
}

GBufferJob::~GBufferJob()
{
}

void GBufferJob::run(GBufferFBO* fbo)
{
    fbo->bind();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_pipeline->bindPipeline();
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    while (!m_queue.empty()) {
        auto object = m_queue.front();
        m_queue.pop();

        auto index_size = object->m_indices.size();
        auto vertex_size = object->m_vertices.size();
        if (!vertex_size) {
            //nothing to render
            continue;
        }

        GLuint vertarray;
        glGenVertexArrays(1, &vertarray);
        glBindVertexArray(vertarray);
        GLuint vertices;
        glGenBuffers(1, &vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertices);
        GLuint indices;
        if (index_size) {
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

        if (!index_size) {
            //generate identity index buffer if no indices found
            glDrawArrays(GL_TRIANGLES, 0, vertex_size);
            glDeleteBuffers(1, &vertices);
        } else {
            //draw elements properly
            glDrawElements(
                GL_TRIANGLES, // mode
                index_size, // count
                GL_UNSIGNED_INT, // type
                (void*)0 // element array buffer offset
            );
            glDeleteBuffers(1, &indices);
            glDeleteBuffers(1, &vertices);
        }
        glDisableVertexAttribArray(0);
    }
}

void GBufferJob::setMVP(const glm::mat4& MVP)
{
    m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "MVP", MVP);
}

void GBufferJob::setView(const glm::mat4& view)
{
    //m_pipeline->setUniform(GLProgram::eShaderType::Vertex, "view", view);
}
