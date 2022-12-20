#include "headers/Window.h"


Window::Window(float width, float height) : m_Width(width), m_Height(height), m_Camera(nullptr)
{
}

Window::~Window()
{
	m_Window = nullptr;
}

void Window::Init()
{
    	/* Initialize the library */
    if (!glfwInit()){
		std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
	}

    // set up width and height if not set
    if (m_Width == 0 || m_Height == 0)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_Width = mode->width;
        m_Height = mode->height;
    }

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Width, m_Height, "Roaller Coaster", NULL, NULL);
    if (!m_Window)
    {
		std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    // user pointer
    glfwSetWindowUserPointer(m_Window, this);

    // set resize callback with user pointer
    glfwSetWindowSizeCallback(m_Window, OnResize);

    /* Init glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		std::cerr << "Failed to initialize glad" << std::endl;
		exit(-1);
    }

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Destroy() const
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::OnResize(GLFWwindow* window, int width, int height)
{
    // get user pointer
    Window* win = (Window*)glfwGetWindowUserPointer(window);
    
    // set new width and height
    win->m_Width = width;
    win->m_Height = height;

    glViewport(0, 0, width, height);

    win->GetCamera()->SetProjection(glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f));

}




