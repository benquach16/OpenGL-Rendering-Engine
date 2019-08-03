#pragma once

#include "../opengldriver/vertexbuffer.h"
#include <vector>

class Mesh {
public:
    Mesh();
    ~Mesh();
    void load(const char* str);
    void render();
    VertexBuffer* getBuffer() { return m_vertexbuffer; }

private:
    VertexBuffer* m_vertexbuffer;
};
