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

Renderer::Renderer(Scene *scene, float width, float height)
:m_Scene(scene), m_Width(width), m_Height(height), m_Window(nullptr)
{
}

Renderer::~Renderer()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
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

	/* Initialize the library */
    if (!glfwInit()){
		std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
	}

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!m_Window)
    {
		std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    /* Init glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		std::cerr << "Failed to initialize glad" << std::endl;
		exit(-1);
    }

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::Render() const
{
	while (!glfwWindowShouldClose(m_Window))
	{
		// clear scene
		Clear();

		// draw scene
		for (auto mesh : m_Scene->GetObjects())
		{
			Draw(*mesh->GetVAO(), *mesh->GetIBO(), *mesh->GetShader());
		}

		// Swap front and back buffers
		glfwSwapBuffers(m_Window);

		// Poll for and process events
		glfwPollEvents();
	}
}
