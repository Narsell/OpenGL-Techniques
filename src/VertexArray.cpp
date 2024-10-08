#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererId);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
    //Bind the vao
    Bind();
    //Bind the vertex buffer
	vb.Bind();

    const auto& attributes = layout.GetElements();
    unsigned int offset = 0;

    //Iterate through attributes for this layout
    for (int i = 0; i < attributes.size(); i++)
    {
        const auto& attributeElement = attributes[i];
        glEnableVertexAttribArray(i);
        //layout.GetStride() gets the full size of 
        glVertexAttribPointer(i, attributeElement.count, attributeElement.type, attributeElement.normalized, layout.GetStride(), (const void*)offset);

        offset += attributeElement.count * VertexBufferElement::GetSizeOfType(attributeElement.type);
    }

}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererId);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
