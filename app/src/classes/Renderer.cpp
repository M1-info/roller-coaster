#include "Renderer.h"

Renderer::Renderer()
	: m_Window(nullptr), m_Camera(nullptr), m_Scene(nullptr), m_FBO(nullptr), m_Light(nullptr)
{
}

Renderer::~Renderer()
{
	m_Window->Destroy();
	delete m_Window.get();
	m_Window = nullptr;

	delete m_FBO.get();
	m_FBO = nullptr;

	delete m_Light.get();
	m_Light = nullptr;

	delete m_UI.get();
	m_UI = nullptr;

	delete m_Scene.get();
	m_Scene = nullptr;
}

void Renderer::Init()
{
	// init window
	m_Window = std::make_shared<Window>();
	m_Window->Init();

	// init Camera
	float fov = 45.0f;
	float aspect = m_Window->GetWidth() / m_Window->GetHeight();
	float near = 0.1f;
	float far = 150.0f;
	m_Camera = std::make_shared<Camera>(fov, aspect, near, far);
	m_Camera->Init();
	m_Window->SetCamera(m_Camera);

	// init lights
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightIntensity = 30.0;
	m_Light = std::make_shared<Light>(lightColor, lightIntensity);
	m_Light->GetTransform()->SetPosition(glm::vec3(0.0f, 90.0f, 0.0f));
	m_Light->GetTransform()->SetIsDirty(true);
	m_Light->GetTransform()->ComputeMatrix();

	// init frame buffer
	m_FBO = std::make_shared<FrameBuffer>(m_Window->GetWidth(), m_Window->GetHeight());
	m_FBO->Init();
	m_Window->SetFBO(m_FBO);

	// init scene
	m_Scene = std::make_shared<Scene>();
	m_Scene->Init();
	m_Scene->SetUpObjectsShaders(m_Light);

	// init UI
	m_UI = std::make_shared<UI>(m_Window, m_Scene, m_Light, m_FBO);
	m_UI->Init();
}

void Renderer::Render()
{

	Skybox *skybox = m_Scene->GetSkybox();
	std::shared_ptr<Cart> cart = std::dynamic_pointer_cast<Cart>(m_Scene->GetObjectByName("Cart"));
	float lastTime = 0.0f;

	Shader *shaderOutline = new Shader("outline");
	glm::mat4 outlinedScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.02f));

	Shader *shaderPhong = new Shader("phong");

	while (!glfwWindowShouldClose(m_Window->GetWindow()))
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		/* CAMERA */
		m_Camera->Render(deltaTime);
		if (m_Camera->m_IsOnCart)
		{
			m_Camera->SetPosition(cart->GetTransform()->GetPosition() + glm::vec3(0.0f, 2.0f, 0.0f));
			m_Camera->Update(cart);
		}

		if (m_UI->GetIsAnimating())
			cart->Animate();

		glm::mat4 viewMatrix = m_Camera->GetView();
		glm::mat4 projectionView = m_Camera->GetProjectionView();
		glm::mat4 projectionViewSkyBox = m_Camera->GetProjection() * glm::mat4(glm::mat3(m_Camera->GetView()));

		/* CLEAR SCREEN AND BIND FBO */
		m_Window->Clear();
		m_FBO->Bind();
		m_Window->Clear();

		/* SKYBOX */
		skybox->GetMaterial()->UpdateShader(projectionViewSkyBox, glm::mat4(0.0f));
		glDisable(GL_DEPTH_TEST);
		skybox->Draw();
		glEnable(GL_DEPTH_TEST);

		/* SCENE OBJECTS */
		for (auto mesh : m_Scene->GetObjects())
		{

			mesh->GetTransform()->ComputeMatrix();

			glm::vec3 cameraPosition = m_Camera->GetPosition();
			glm::vec3 lightPosition = m_Light->GetTransform()->GetPosition();
			glm::mat4 modelMatrix = mesh->GetTransform()->GetMatrix();

			if (mesh->GetType() == MeshType::RAILS)
				mesh->GetMaterial()->UpdateShader(projectionView, modelMatrix);
			else
				mesh->GetMaterial()->UpdateShader(projectionView, modelMatrix, viewMatrix, m_Light, cameraPosition);

			/* OBJECT CHILDREN */
			if (mesh->GetChildren().size() > 0)
			{
				if (mesh->GetType() == MeshType::RAILS)
				{
					std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(mesh);

					if (rails->m_DrawRails)
						for (auto rail : rails->GetRails())
						{
							rail->GetTransform()->ComputeMatrix();
							glm::mat4 modelMatrix = rail->GetTransform()->GetMatrix();
							rail->GetMaterial()->UpdateShader(projectionView, modelMatrix, viewMatrix, m_Light, cameraPosition);

							for (auto child : rail->GetChildren())
							{
								child->GetTransform()->ComputeMatrix();
								glm::mat4 modelMatrix = child->GetTransform()->GetMatrix();
								child->GetMaterial()->UpdateShader(projectionView, modelMatrix, viewMatrix, m_Light, cameraPosition);
							}
						}
				}

				for (auto child : mesh->GetChildren())
				{
					child->GetTransform()->ComputeMatrix();
					glm::mat4 modelMatrix;

					if (mesh->GetType() == MeshType::RAILS)
					{
						modelMatrix = glm::mat4(1.0f);
						child->GetMaterial()->UpdateShader(projectionView, modelMatrix);
					}
					else
					{
						modelMatrix = child->GetTransform()->GetMatrix();
						child->GetMaterial()->UpdateShader(projectionView, modelMatrix, viewMatrix, m_Light, cameraPosition);
					}
				}
			}

			// if object is selected, use stencil buffer to draw outline
			if (mesh->m_IsSelected)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
			}

			// draw object with own shader
			mesh->Draw();

			if (mesh->m_IsSelected)
			{
				mesh->GetMaterial()->SetShader(shaderOutline);
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.02f));
				mesh->GetMaterial()->UpdateShader(projectionView, modelMatrix * outlinedScaleMatrix);
				mesh->Draw();
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);
				mesh->GetMaterial()->SetShader(shaderPhong);
			}
		}

		// Unbind FBO after drawind scene on it
		m_FBO->Unbind();

		// Draw UI after FBO is unbound (so it's not drawn on FBO)
		m_UI->Render();

		// Swap front and back buffers
		m_Window->SwapBuffers();

		// Poll process events
		m_Window->PollEvents();
	}
}
