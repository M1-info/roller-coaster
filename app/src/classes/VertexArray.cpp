#include "headers/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout)
{
	Bind();
	vbo.Bind();
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto &element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, 
			element.count, 
			element.type, 
			element.normalized, 
			layout.GetStride(), 
			reinterpret_cast<void*>(offset + offsetof(VertexAttributes, position)));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}