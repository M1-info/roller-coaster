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
:m_Scene(scene), m_Window(nullptr)
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
	glClear(GL_COLOR_BUFFER_BIT);
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{

	shader.Bind();
	vao.Bind();
	ibo.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	vao.Unbind();
	ibo.Unbind();
}

void Renderer::Init()
{
	m_Window = new Window();
	m_Window->Init();

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
}

void Renderer::Render() const
{
	while (!glfwWindowShouldClose(m_Window->GetWindow()))
	{
		// clear scene
		Clear();

		// draw scene
		for (auto mesh : m_Scene->GetObjects())
		{
			mesh->GetShader()->SetUniformMat4f("u_projection", m_Camera->GetProjection());
			Draw(*mesh->GetVAO(), *mesh->GetIBO(), *mesh->GetShader());
		}

		// Swap front and back buffers
		glfwSwapBuffers(m_Window->GetWindow());

		// Poll for and process events
		glfwPollEvents();
	}
}
