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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void Renderer::Draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader) const
{

	shader.Bind();
	vao.Bind();
	ibo.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	vao.Unbind();
	ibo.Unbind();
	shader.Unbind();
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

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Camera->Init(position, target, up);

	m_Window->SetCamera(m_Camera);

	// init UI
	m_UI = new UI();
}

void Renderer::Render() const
{

	auto *skybox = m_Scene->GetSkybox();
	skybox->GetShader()->Bind();
	skybox->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
	skybox->GetShader()->SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_Camera->GetView())));
	skybox->GetShader()->SetUniform1i("u_texture", 0);
	skybox->GetShader()->Unbind();

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

		// draw skybox
		skybox->GetShader()->Bind();
		skybox->GetShader()->SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_Camera->GetView())));
		skybox->GetShader()->Unbind();
		skybox->Draw(m_Camera->GetProjection(), m_Camera->GetView());

		// draw scene
		for (auto mesh : m_Scene->GetObjects())
		{
			mesh->GetShader()->Bind();
			mesh->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
			mesh->GetShader()->SetUniformMat4f("u_view", m_Camera->GetView());
			mesh->GetShader()->SetUniformMat4f("u_model", mesh->ComputeMatrix());
			Draw(*mesh->GetVAO(), *mesh->GetIBO(), *mesh->GetShader());
		}

		// draw UI
		m_UI->Render();

		// Swap front and back buffers
		glfwSwapBuffers(m_Window->GetWindow());

		// Poll for and process events
		glfwPollEvents();
	}
}
