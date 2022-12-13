#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include <vector>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class Renderer
{
private:
	GLFWwindow* m_Window;
	float m_Width, m_Height;

public:
	Renderer() = default;
	Renderer(float width, float height);
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	void Init();
	void Render(std::vector<Mesh*> &meshes) const;

	inline GLFWwindow* GetWindow() const { return m_Window; }
};


#endif // RENDERER_H