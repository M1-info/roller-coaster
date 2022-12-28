#include "headers/Window.h"

Window::Window(float width, float height)
    : m_Width(width), m_Height(height), m_Camera(nullptr), m_Window(nullptr), m_IsMousePressed(0)
{
}

Window::~Window()
{
    m_Window = nullptr;
}

void Window::Init()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    // set up width and height if not set
    if (m_Width == 0 || m_Height == 0)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
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
    glEnable(GL_DEPTH_TEST);
}

void Window::Destroy() const
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::OnResize(GLFWwindow *window, int width, int height)
{
    // get user pointer
    Window *win = (Window *)glfwGetWindowUserPointer(window);

    // set new width and height
    win->m_Width = width;
    win->m_Height = height;

    glViewport(0, 0, width, height);

    // update camera
    float aspect;
    if(width == 0 || height == 0)
        aspect = 1;
    else
        aspect = (float)width / (float)height;

    win->GetCamera()->SetAspectRatio(aspect);

    float fov = win->GetCamera()->GetFov();
    float near = win->GetCamera()->GetNearPlane();
    float far = win->GetCamera()->GetFarPlane();
    win->GetCamera()->SetProjection(glm::perspective(fov, aspect, near, far));
}

void Window::OnMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    Window *win = (Window *)glfwGetWindowUserPointer(window);

    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    float offsetX = x - win->GetCamera()->GetLastX();
    float offsetY = win->GetCamera()->GetLastY() - y;

    win->GetCamera()->SetLastX(x);
    win->GetCamera()->SetLastY(y);

    if (!win->GetIsMousePressed())
        return;

    win->GetCamera()->OnMouseMove(offsetX, offsetY);
}

void Window::OnMouseClick(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos); //getting cursor position

    Window *win = (Window *)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        win->m_IsMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        win->m_IsMousePressed = false;
    }

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, win->GetFBO());
    // glReadBuffer(GL_COLOR_ATTACHMENT0);

    // PixelInfo Pixel;
    // glReadPixels(xpos, ypos, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

    // glReadBuffer(GL_NONE);
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    // //print the pixel info
    // std::cout << "Pixel Info: " << Pixel.ObjectID << " " << Pixel.DrawID << " " << Pixel.PrimID << std::endl;
}

void Window::OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Window *win = (Window *)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (action == GLFW_RELEASE)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::NOMOVE);
        return;
    }

    // camera movement
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::FORWARD);
        return;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::BACKWARD);
        return;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::LEFT);
        return;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::RIGHT);
        return;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::UP);
        return;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        win->GetCamera()->SetCurrentMovementDirection(CameraMovement::DOWN);
        return;
    }
}
