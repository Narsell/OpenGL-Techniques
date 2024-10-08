#include "VertexBuffer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //Generates 1 buffer name into m_RendererId. https://docs.gl/gl4/glGenBuffers
    glGenBuffers(1, &m_RendererId);
    //Binds the m_RendererId to the specified target (GL_ARRAY_BUFFER). https://docs.gl/gl4/glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    //Basically sets the data to the object binded to GL_ARRAY_BUFFER, which is m_RendererId in this case. https://docs.gl/gl4/glBufferData
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::Bind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}

void VertexBuffer::Unbind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
