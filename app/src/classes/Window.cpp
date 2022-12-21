#include "headers/Window.h"


Window::Window(float width, float height) 
: m_Width(width), m_Height(height), m_Camera(nullptr), m_Window(nullptr), m_IsMousePressed(0), m_MouseSensitivity(0.1f)
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

    if(m_MouseSensitivity == 0.0f)
        m_MouseSensitivity = 0.01f;

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
    glfwSetScrollCallback(m_Window, OnMouseScroll);
    glfwSetCursorPosCallback(m_Window, OnMouseMove);
    glfwSetMouseButtonCallback(m_Window, OnMouseClick);
    glfwSetKeyCallback(m_Window, OnKeyPress);

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

    // update camera
    float aspect = (float)width / (float)height;
    win->GetCamera()->SetAspectRatio(aspect);

    float fov = win->GetCamera()->GetFov();
    float near = win->GetCamera()->GetNearPlane();
    float far = win->GetCamera()->GetFarPlane();
    win->GetCamera()->SetProjection(glm::perspective(fov, aspect, near, far));

}

void Window::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    Window* win = (Window*)glfwGetWindowUserPointer(window);

    if (win->GetIsMousePressed())
    {;
        xpos *= win->m_MouseSensitivity;
        ypos *= win->m_MouseSensitivity;

        float yaw = win->GetCamera()->GetYaw() + xpos;
        float pitch = win->GetCamera()->GetPitch() + ypos;

        pitch = std::clamp(pitch, -89.0f, 89.0f);

        win->GetCamera()->SetYaw(yaw);
        win->GetCamera()->SetPitch(pitch);

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        win->GetCamera()->SetFront(glm::normalize(front));

        glm::vec3 right = glm::normalize(glm::cross(front, win->GetCamera()->GetWorldUp()));
        win->GetCamera()->SetRight(right);

        glm::vec3 up = glm::normalize(glm::cross(right, front));
        win->GetCamera()->SetUp(up);

        win->GetCamera()->SetView(glm::lookAt(win->GetCamera()->GetPosition(), 
                                              win->GetCamera()->GetPosition() + front, 
                                              up));
    }
}

void Window::OnMouseClick(GLFWwindow* window, int button, int action, int mods)
{
    Window* win = (Window*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        win->m_IsMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        win->m_IsMousePressed = false;
    }
}

void Window::OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* win = (Window*)glfwGetWindowUserPointer(window);

    float fov = win->GetCamera()->GetFov() + yoffset;

    fov = std::clamp(fov, 1.0f, 45.0f);
    std::cout << fov << std::endl;

    win->m_Camera->SetFov(fov);
    float aspect = win->GetCamera()->GetAspectRatio();
    float near = win->GetCamera()->GetNearPlane();
    float far = win->GetCamera()->GetFarPlane();
    win->GetCamera()->SetProjection(glm::perspective(fov, aspect, near, far));
}

void Window::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* win = (Window*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(action == GLFW_RELEASE){
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::NOMOVE);
        return;
    }

    // camera movement
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::FORWARD);
        return;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::BACKWARD);
        return;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::LEFT);
        return;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::RIGHT);
        return;
    }
}




