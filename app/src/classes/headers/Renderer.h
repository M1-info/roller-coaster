#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#define GRAVITY 9.81f
#define DELTA_TIME_CART_RENDER 0.01f

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <vector>
#include <memory>

#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "UI.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Rails.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "OBJLoader.h"

#define ASSERT(x) \
	if (!(x))     \
		__debugbreak();
#define GLCall(x)   \
	GLClearError(); \
	x;              \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);

class VertexArray;
class Renderer
{
public:
	Renderer() = default;
	Renderer(Scene *scene);
	~Renderer();

	void Clear() const;
	void Init();
	void Render();
	void SetUpScene();

	inline void SetScene(Scene *scene) { m_Scene = scene; }
	inline Scene *GetScene() const { return m_Scene; }
	inline std::shared_ptr<Camera> GetCamera() const { return m_Camera; }
	inline Window *GetWindow() const { return m_Window; }
	inline UI *GetUI() const { return m_UI; }

private:
	Window *m_Window;
	Scene *m_Scene;
	std::shared_ptr<Camera> m_Camera;
	Light *m_Light;
	UI *m_UI;
	FrameBuffer *m_FBO;
};

#endif // RENDERER_H