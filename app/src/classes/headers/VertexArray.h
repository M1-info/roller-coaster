#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexBufferLayout;

struct VertexAttributes
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoords[2];
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

#endif // VERTEXARRAY_H