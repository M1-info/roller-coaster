#include "headers/Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

bool GLLogCall(const char *function, const char *file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer(Scene *scene)
	: m_Scene(scene), m_Window(nullptr)
{
}

Renderer::~Renderer()
{
	m_Window->Destroy();
	delete m_Window;
	m_Window = nullptr;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void Renderer::Init()
{
	// init window
	m_Window = new Window();
	m_Window->Init();

	// init camera
	float fov = 45.0f;
	float aspect = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
	float near = 0.1f;
	float far = 100.0f;
	m_Camera = std::make_shared<Camera>(fov, aspect, near, far);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 8.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Camera->Init(position, target, up);

	m_Window->SetCamera(m_Camera);

	// init UI
	m_UI = new UI();

	// init lights
	m_Light = new Light(glm::vec3(5.0f, 10.0f, 0.0f), glm::vec3(0.0), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);

	m_UI->SetLight(m_Light);

	// init frame buffer
	m_FBO = new FrameBuffer(m_Window->GetWidth(), m_Window->GetHeight());
	m_FBO->Init();
	m_UI->SetFBO(m_FBO);
	m_Window->SetFBO(m_FBO);
}

void Renderer::Render()
{

	Skybox *skybox = m_Scene->GetSkybox();
	std::vector<std::shared_ptr<Mesh>> meshes = m_Scene->GetObjects();
	SetUpScene(skybox, meshes);

	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(m_Window->GetWindow()))
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// update camera
		m_Camera->Render(deltaTime);

		// clear scene
		Clear();
		m_FBO->Bind();
		Clear();

		// draw skybox
		skybox->GetShader()->Bind();
		skybox->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
		skybox->GetShader()->SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_Camera->GetView())));
		skybox->GetShader()->Unbind();
		glDisable(GL_DEPTH_TEST);
		skybox->Draw();
		glEnable(GL_DEPTH_TEST);
	
		// draw scene
		for (auto mesh : m_Scene->GetObjects())
		{
			Material *material = mesh->GetMaterial();

			Shader *shader = material->GetShader();
			
			shader->Bind();


			if(mesh->GetType() != MeshType::RAILS){
				glm::vec3 cameraPosition = m_Camera->GetPosition();
				glm::vec3 lightPosition = m_Light->m_Position;
				shader->SetUniform3f("u_cameraPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
				shader->SetUniform3f("u_light.position", lightPosition.x, lightPosition.y, lightPosition.z);
			}

			shader->SetUniformMat4f("u_projection", m_Camera->GetProjection());
			shader->SetUniformMat4f("u_view", m_Camera->GetView());
			shader->SetUniformMat4f("u_model", mesh->ComputeMatrix());

			if(mesh->m_IsSelected){
				shader->SetUniform1i("u_isSelected", 1);
			} else {
				shader->SetUniform1i("u_isSelected", 0);
			}


			shader->Unbind();

			if(mesh->GetChildren().size() > 0){
				for(auto child : mesh->GetChildren()){
					child->GetMaterial()->GetShader()->Bind();
					child->GetMaterial()->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
					child->GetMaterial()->GetShader()->SetUniformMat4f("u_view", m_Camera->GetView());
					child->GetMaterial()->GetShader()->SetUniformMat4f("u_model", child->ComputeMatrix());
					if(child->m_IsSelected){
						child->GetMaterial()->GetShader()->SetUniform1i("u_isSelected", 1);
					} else {
						child->GetMaterial()->GetShader()->SetUniform1i("u_isSelected", 0);
					}

					child->GetMaterial()->GetShader()->Unbind();
				}
			}

			mesh->Draw();

		}

		m_FBO->Unbind();

		// draw UI
		m_UI->Render();

		// Swap front and back buffers
		glfwSwapBuffers(m_Window->GetWindow());

		// Poll for and process events
		glfwPollEvents();

	}
}

void Renderer::SetUpScene(Skybox * skybox, std::vector<std::shared_ptr<Mesh>> meshes)
{

	skybox->GetShader()->Bind();
	skybox->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
	skybox->GetShader()->SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_Camera->GetView())));
	skybox->GetShader()->SetUniform1i("u_texture", 0);
	skybox->GetShader()->Unbind();

	for (auto mesh : meshes)
	{
		if(mesh->GetType() == MeshType::RAILS)
			continue;

		Material *material = mesh->GetMaterial();

		Shader *shader = material->GetShader();

		shader->Bind();

		// pass material infos
		Color materialAmbient = material->GetAmbientColor();
		Color materialDiffuse = material->GetDiffuseColor();
		Color materialSpecular = material->GetSpecularColor();
		float specularExponent = material->GetSpecularExponent();

		shader->SetUniform3f("u_material.coeffAmbient", materialAmbient.r, materialAmbient.g, materialAmbient.b);
		shader->SetUniform3f("u_material.coeffDiffuse", materialDiffuse.r, materialDiffuse.g, materialDiffuse.b);
		shader->SetUniform3f("u_material.coeffSpecular", materialSpecular.r, materialSpecular.g, materialSpecular.b);
		shader->SetUniform1f("u_material.specularExponent", specularExponent);

		glm::vec3 lightPosition = m_Light->m_Position;
		glm::vec3 lightColor = m_Light->m_Color;

		shader->SetUniform1f("u_light.intensity", m_Light->GetIntensity());
		shader->SetUniform3f("u_light.position", lightPosition.x, lightPosition.y, lightPosition.z);
		shader->SetUniform3f("u_light.color", lightColor.x, lightColor.y, lightColor.z);
		
		shader->Unbind();
	}

}
