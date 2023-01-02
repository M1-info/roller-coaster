#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void SetData(const void* data, unsigned int size);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

#endif // VERTEXBUFFER_H