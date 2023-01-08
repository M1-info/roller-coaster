#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include "buffers/FrameBuffer.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "ui/UI.h"

#include "meshes/Mesh.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	/**
	 * @brief Initialize the renderer and all the objects it contains
	 *
	 */
	void Init();

	/**
	 * @brief Render the scene (the main loop)
	 *
	 */
	void Render();

private:
	/**
	 * @brief Window object
	 * 	Rendered scene will be displayed in this window
	 */
	std::shared_ptr<Window> m_Window;

	/**
	 * @brief Scene object
	 * 	Scene contains all objects that will be rendered
	 */
	std::shared_ptr<Scene> m_Scene;

	/**
	 * @brief Camera object
	 * 	Camera object is used to render scene from a specific point of view
	 */
	std::shared_ptr<Camera> m_Camera;

	/**
	 * @brief Light object
	 * 	Light object is used to render scene with a phong model
	 */
	std::shared_ptr<Light> m_Light;

	/**
	 * @brief UI object
	 * 	UI object is used to render all the interface to interact with the scene
	 */
	std::shared_ptr<UI> m_UI;

	/**
	 * @brief FrameBuffer object
	 * 	FrameBuffer object is used to render the scene in a texture in the UI
	 */
	std::shared_ptr<FrameBuffer> m_FBO;
};

#endif // RENDERER_H