#include "headers/VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout, int layout_start_index)
{
	Bind();
	vbo.Bind();
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;
	int i = layout_start_index;
	for(const auto &element : elements)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, 
			element.count, 
			element.type, 
			element.normalized, 
			layout.GetStride(), 
			reinterpret_cast<void*>(offset + offsetof(VertexAttributes, position)));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		i++;
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